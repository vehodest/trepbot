#pragma once
#include <string>
#include <vector>

class Httper {
 public:
  using ContainerType = std::vector<char>;
  using httpCode = long;

  virtual void SetTimeout(size_t msec) = 0;

  virtual httpCode Get(std::string const& url, ContainerType& answer) = 0;

  virtual httpCode Post(std::string const& url,
                        std::string const& partname,
                        std::string const& filename,
                        ContainerType const& data,
                        ContainerType answer = ContainerType()) = 0;

  virtual httpCode Put(std::string const& url,
                       ContainerType const& data,
                       ContainerType& answer) = 0;

  virtual httpCode Delete(std::string const& url, ContainerType& answer) = 0;

  virtual std::string Redirect(std::string const& url,
                               ContainerType& answer) = 0;

  virtual void AddHeader(std::string const& headers) = 0;

  virtual void ClearHeaders() = 0;
};