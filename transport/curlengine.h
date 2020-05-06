#pragma once
#include <escaper.h>
#include <httper.h>
#include <stopflag.h>

#include <memory>
#include <vector>

#include <curl/curl.h>

class CurlEngine final : public Httper, public Escaper {
 public:
  CurlEngine(StopFlag& stopFlag);
  CurlEngine(CurlEngine const&) = delete;
  CurlEngine& operator=(CurlEngine const&) = delete;

  std::string Escape(std::string const& str) override;
  void SetTimeout(size_t msec) override;
  ContainerType Get(std::string const& url) override;
  ContainerType Post(std::string const& url,
                     std::string const& partname,
                     std::string const& filename,
                     ContainerType const& data) override;

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

  static size_t WriteCallback(char* ptr,
                              size_t size,
                              size_t nmemb,
                              void* userdata);

  static int ProgressCallback(void* clientp,
                              curl_off_t dltotal,
                              curl_off_t dlnow,
                              curl_off_t ultotal,
                              curl_off_t ulnow);

  void PrepareRequest(std::string const& url, ContainerType& data);
  void Perform(std::string const& info);
};