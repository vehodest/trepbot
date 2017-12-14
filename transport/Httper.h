#pragma once
#include <string>
#include <vector>

class Httper {
 public:
  typedef std::vector<char> ContainerType;

  virtual void SetTimeout(size_t msec) = 0;
  virtual ContainerType Get(std::string const& url) = 0;
  virtual ContainerType Post(std::string const& url,
                             std::string const& partname,
                             std::string const& filename,
                             ContainerType const& data) = 0;
};