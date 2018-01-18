#pragma once

#include "Escaper.h"
#include "Httper.h"
#include "StopFlag.h"

#include <initializer_list>
#include <map>
#include <memory>

#include <curl/curl.h>

class CurlEngine final : public Httper, public Escaper {
 public:
  CurlEngine(StopFlag& stopFlag);
  CurlEngine(CurlEngine const&) = delete;
  CurlEngine& operator=(CurlEngine const&) = delete;

  std::string Escape(std::string const& str) override;

  void SetTimeout(size_t msec) override;

  httpCode Get(std::string const& url, ContainerType& answer) override;

  httpCode Post(std::string const& url,
                std::string const& partname,
                std::string const& filename,
                ContainerType const& data,
                ContainerType answer = ContainerType()) override;

  httpCode Put(std::string const& url,
               ContainerType const& data,
               ContainerType& answer) override;

  httpCode Delete(std::string const& url, ContainerType& answer) override;

  std::string Redirect(std::string const& url, ContainerType& answer) override;

  void AddHeader(std::string const& header) override;

  void ClearHeaders() override;

 private:
  StopFlag& stopFlag;
  std::unique_ptr<CURL, void (*)(CURL*)> handle;
  char errorMessage[CURL_ERROR_SIZE];
  bool isVerbose;
  size_t timeoutMsec;

  static class CurlGlobalInit final {
   public:
    CurlGlobalInit();
    ~CurlGlobalInit();
  } curlGlobalInit;

  class ResponseHeaders final {
   public:
    void ClearValues();
    void RemoveAll();
    void AddHeader(std::string const& header);
    bool IsPresent(std::string const& header) const;
    std::string GetHeader(std::string const& header) const;
    bool Parse(char const* data, size_t length);

   private:
    std::map<std::string, std::string> headers;
  } responseHeaders;

  class RequestHeaders final {
   public:
    RequestHeaders() : headers(nullptr, curl_slist_free_all) {}

    void Add(std::string const& fullHeader) {
      headers.reset(::curl_slist_append(headers.get(), fullHeader.c_str()));
      if (nullptr == headers.get()) {
        throw std::runtime_error("Can't add HTTP header to request");
      }
    }

    curl_slist const* Get() { return headers.get(); }

    void Clear() { headers.reset(); }

    bool IsEmpty() { return nullptr == headers.get(); }

   private:
    std::unique_ptr<curl_slist, void (*)(curl_slist*)> headers;
  } requestHeaders;

  struct CallbackContext final {
    CallbackContext(ContainerType const& data) : data(data), position(0) {}

    ContainerType const& data;
    size_t position;
  };

  class ResponseHeadersHelper {
   public:
    ResponseHeadersHelper(CurlEngine::ResponseHeaders& headers,
                          std::initializer_list<std::string> l)
        : headers(headers) {
      headers.RemoveAll();
      for (auto const& elem : l) {
        headers.AddHeader(elem);
      }
    }

    ~ResponseHeadersHelper() { headers.RemoveAll(); }

   private:
    CurlEngine::ResponseHeaders& headers;
  };

  static size_t WriteCallback(char* ptr,
                              size_t size,
                              size_t nmemb,
                              void* userdata);

  static size_t ReadCallback(char* buffer,
                             size_t size,
                             size_t nitems,
                             void* instream);

  static int ProgressCallback(void* clientp,
                              curl_off_t dltotal,
                              curl_off_t dlnow,
                              curl_off_t ultotal,
                              curl_off_t ulnow);

  static size_t HeaderCallback(char* buffer,
                               size_t size,
                               size_t nitems,
                               void* userdata);

  void PrepareRequest(std::string const& url, ContainerType& data);
  httpCode Perform(std::string const& info);
};