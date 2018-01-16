#include "CurlEngine.h"
#include <cstring>
#include <iostream>

CurlEngine::CurlGlobalInit CurlEngine::curlGlobalInit;

CurlEngine::CurlGlobalInit::CurlGlobalInit() {
  ::curl_global_init(CURL_GLOBAL_DEFAULT);
}

CurlEngine::CurlGlobalInit::~CurlGlobalInit() {
  ::curl_global_cleanup();
}

void CurlEngine::ResponseHeaders::ClearValues() {
  for (auto& h : headers) {
    h.second.clear();
  }
}

void CurlEngine::ResponseHeaders::RemoveAll() {
  headers.clear();
}

void CurlEngine::ResponseHeaders::AddHeader(std::string const& header) {
  auto it = headers.find(header);
  if (headers.end() != it) {
    return;
  }

  headers.insert({header, std::string()});
}

bool CurlEngine::ResponseHeaders::IsPresent(std::string const& header) const {
  return !GetHeader(header).empty();
}

std::string CurlEngine::ResponseHeaders::GetHeader(
    std::string const& header) const {
  auto it = headers.find(header);
  if (headers.end() == it) {
    return std::string();
  }

  return it->second;
}

bool CurlEngine::ResponseHeaders::Parse(char const* data, size_t length) {
  std::string str(data, length);
  std::string::size_type delim_pos = str.find(':');
  if (std::string::npos == delim_pos || 0 == delim_pos) {
    return false;
  }

  std::string name = str.substr(0, delim_pos);
  auto it = headers.find(name);
  if (headers.end() == it) {
    return false;
  }

  std::string::size_type beginPos = delim_pos + 1;
  while (true) {
    if (beginPos >= str.size()) {
      return false;
    }

    if (' ' != str[beginPos]) {
      break;
    }
    ++beginPos;
  }

  std::string::size_type valueLen = str.size() - beginPos - 1;
  while (true) {
    if (valueLen == 0) {
      return false;
    }

    char c = str[beginPos + valueLen - 1];
    if ('\r' != c && '\n' != c) {
      break;
    }
    --valueLen;
  }

  it->second = str.substr(beginPos, valueLen);
}

CurlEngine::CurlEngine(StopFlag& stopFlag)
    : stopFlag(stopFlag),
      handle(::curl_easy_init(), ::curl_easy_cleanup),
      isVerbose(false),
      timeoutMsec(30000) {
  if (!handle) {
    throw std::runtime_error("Error when curl_easy_init");
  }

  responseHeaders.AddHeader("Location");
}

std::string CurlEngine::Escape(std::string const& str) {
  std::unique_ptr<char[], void (*)(void*)> esc(
      ::curl_easy_escape(handle.get(), str.c_str(), str.length()), ::curl_free);
  if (!esc) {
    throw std::runtime_error("Can't escape string");
  }

  return std::string(esc.get());
}

size_t CurlEngine::WriteCallback(char* ptr,
                                 size_t size,
                                 size_t nmemb,
                                 void* userdata) {
  ContainerType& data = *static_cast<ContainerType*>(userdata);

  size_t totalSize = size * nmemb;
  data.insert(data.end(), ptr, ptr + totalSize);

  return totalSize;
}

size_t CurlEngine::ReadCallback(char* buffer,
                                size_t size,
                                size_t nitems,
                                void* instream) {
  CallbackContext& context = *static_cast<CallbackContext*>(instream);
  size_t totalSize = size * nitems;

  if (context.position + totalSize > context.data.size()) {
    if (context.data.size() > context.position) {
      totalSize = context.data.size() - context.position;
    } else {
      totalSize = 0;
    }
  }

  if (totalSize > 0) {
    std::memcpy(buffer, context.data.data() + context.position, totalSize);
    context.position += totalSize;
  }

  return totalSize;
}

int CurlEngine::ProgressCallback(void* clientp,
                                 curl_off_t dltotal,
                                 curl_off_t dlnow,
                                 curl_off_t ultotal,
                                 curl_off_t ulnow) {
  StopFlag& stopFlag = *static_cast<StopFlag*>(clientp);
  return stopFlag.IsStop() ? 1 : 0;
}

CurlEngine::httpCode CurlEngine::Perform(std::string const& info) {
  CURLcode result = ::curl_easy_perform(handle.get());
  long httpCode(0);
  ::curl_easy_getinfo(handle.get(), CURLINFO_RESPONSE_CODE, &httpCode);

  if (CURLE_OK == result) {
    return httpCode;
  }

  std::string msg = info + "; HTTP code: " + std::to_string(httpCode) +
                    "; Message: " + errorMessage;
  throw std::runtime_error(msg);
}

size_t CurlEngine::HeaderCallback(char* buffer,
                                  size_t size,
                                  size_t nitems,
                                  void* userdata) {
  size_t bytes = size * nitems;
  static_cast<CurlEngine*>(userdata)->responseHeaders.Parse(buffer, bytes);
  return bytes;
}

void CurlEngine::PrepareRequest(std::string const& url, ContainerType& data) {
  ::curl_easy_reset(handle.get());

  ::curl_easy_setopt(handle.get(), CURLOPT_URL, url.c_str());

  ::curl_easy_setopt(handle.get(), CURLOPT_VERBOSE, isVerbose);

  ::curl_easy_setopt(handle.get(), CURLOPT_TIMEOUT_MS,
                     static_cast<long>(timeoutMsec));

  ::curl_easy_setopt(handle.get(), CURLOPT_WRITEFUNCTION, WriteCallback);
  ::curl_easy_setopt(handle.get(), CURLOPT_WRITEDATA, &data);

  ::curl_easy_setopt(handle.get(), CURLOPT_NOPROGRESS, 0L);
  ::curl_easy_setopt(handle.get(), CURLOPT_XFERINFOFUNCTION, ProgressCallback);
  ::curl_easy_setopt(handle.get(), CURLOPT_XFERINFODATA, &stopFlag);

  ::curl_easy_setopt(handle.get(), CURLOPT_HEADERFUNCTION, HeaderCallback);
  ::curl_easy_setopt(handle.get(), CURLOPT_HEADERDATA, this);

  errorMessage[0] = '\0';
  ::curl_easy_setopt(handle.get(), CURLOPT_ERRORBUFFER, errorMessage);

  if (!requestHeaders.IsEmpty()) {
    ::curl_easy_setopt(handle.get(), CURLOPT_HTTPHEADER, requestHeaders.Get());
  }
}

void CurlEngine::SetTimeout(size_t msec) {
  timeoutMsec = msec;
}

CurlEngine::ContainerType CurlEngine::Get(std::string const& url) {
  ContainerType data;
  PrepareRequest(url, data);

  Perform("GET request");

  return data;
}

CurlEngine::ContainerType CurlEngine::Post(
    std::string const& url,
    std::string const& partname,
    std::string const& filename,
    CurlEngine::ContainerType const& data) {
  ContainerType result;
  PrepareRequest(url, result);

  struct curl_httppost* post = NULL;
  struct curl_httppost* last = NULL;
  ::curl_formadd(&post, &last, CURLFORM_COPYNAME, partname.c_str(),
                 CURLFORM_BUFFER, filename.c_str(), CURLFORM_BUFFERPTR,
                 data.data(), CURLFORM_BUFFERLENGTH, data.size(), CURLFORM_END);
  std::unique_ptr<curl_httppost, void (*)(curl_httppost*)> post_handle(
      post, curl_formfree);

  if (!post_handle) {
    throw std::runtime_error("Can't add form");
  }

  ::curl_easy_setopt(handle.get(), CURLOPT_HTTPPOST, post_handle.get());

  Perform("POST request");

  return result;
}

CurlEngine::httpCode CurlEngine::Put(std::string const& url,
                                     ContainerType const& data) {
  ContainerType answer;
  PrepareRequest(url, answer);

  CallbackContext context(data);
  ::curl_easy_setopt(handle.get(), CURLOPT_READFUNCTION, ReadCallback);
  ::curl_easy_setopt(handle.get(), CURLOPT_READDATA, &context);
  ::curl_easy_setopt(handle.get(), CURLOPT_UPLOAD, 1L);
  ::curl_easy_setopt(handle.get(), CURLOPT_INFILESIZE_LARGE,
                     static_cast<curl_off_t>(data.size()));

  return Perform("PUT request");
}

CurlEngine::httpCode CurlEngine::Delete(std::string const& url) {
  ContainerType answer;
  PrepareRequest(url, answer);

  ::curl_easy_setopt(handle.get(), CURLOPT_CUSTOMREQUEST, "DELETE");

  return Perform("DELETE request");
}

std::string CurlEngine::Redirect(std::string const& url) {
  static const std::string Location("Location");

  ResponseHeadersHelper helper(responseHeaders, {Location});
  Get(url);
  if (!responseHeaders.IsPresent(Location)) {
    throw std::runtime_error("Error result of redirection GET request");
  }

  return responseHeaders.GetHeader(Location);
}

void CurlEngine::AddHeader(std::string const& header) {
  requestHeaders.Add(header);
}

void CurlEngine::ClearHeaders() {
  requestHeaders.Clear();
}
