#include "CurlEngine.h"
#include <sstream>
#include <iostream>

CurlEngine::CurlGlobalInit CurlEngine::curlGlobalInit;

CurlEngine::CurlGlobalInit::CurlGlobalInit() {
  curl_global_init(CURL_GLOBAL_DEFAULT);
}

CurlEngine::CurlGlobalInit::~CurlGlobalInit() {
  curl_global_cleanup();
}

CurlEngine::CurlEngine(StopFlag& stopFlag)
    : stopFlag(stopFlag), handle(::curl_easy_init(), ::curl_easy_cleanup), isVerbose(false), timeoutMsec(30000) {
  if (!handle) {
    throw std::runtime_error("Error when curl_easy_init");
  }
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
  ContainerType& voice = *static_cast<ContainerType*>(userdata);

  size_t totalSize = size * nmemb;
  voice.insert(voice.end(), ptr, ptr + totalSize);

  return totalSize;
}

int CurlEngine::ProgressCallback(void* clientp,
                                 curl_off_t dltotal,
                                 curl_off_t dlnow,
                                 curl_off_t ultotal,
                                 curl_off_t ulnow) {
  StopFlag &stopFlag = *static_cast<StopFlag*>(clientp);
  return stopFlag.IsStop() ? 1 : 0;
}

void CurlEngine::Perform(std::string const& info) {
  CURLcode result = ::curl_easy_perform(handle.get());
  long httpCode(0);
  curl_easy_getinfo (handle.get(), CURLINFO_RESPONSE_CODE, &httpCode);

  if (CURLE_OK == result) {
    return;
  }

  std::stringstream str;
  str << info << "; HTTP code: " << httpCode << "; Message: " << errorMessage;
  throw std::runtime_error(str.str());
}

void CurlEngine::PrepareRequest(std::string const& url, ContainerType& data) {
  ::curl_easy_reset(handle.get());

  ::curl_easy_setopt(handle.get(), CURLOPT_URL, url.c_str());

  ::curl_easy_setopt(handle.get(), CURLOPT_VERBOSE, isVerbose);

  ::curl_easy_setopt(handle.get(), CURLOPT_TIMEOUT_MS, static_cast<long>(timeoutMsec));

  ::curl_easy_setopt(handle.get(), CURLOPT_WRITEFUNCTION, WriteCallback);
  ::curl_easy_setopt(handle.get(), CURLOPT_WRITEDATA, &data);

  ::curl_easy_setopt(handle.get(), CURLOPT_NOPROGRESS, 0);
  ::curl_easy_setopt(handle.get(), CURLOPT_XFERINFOFUNCTION, ProgressCallback);
  ::curl_easy_setopt(handle.get(), CURLOPT_XFERINFODATA, &stopFlag);

  errorMessage[0] = '\0';
  ::curl_easy_setopt(handle.get(), CURLOPT_ERRORBUFFER, errorMessage);
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