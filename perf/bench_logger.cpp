#include <benchmark/benchmark.h>

#include <system_error>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFSIZE 4096

// try to optimize without removing syscalls!
class Logger {
public:
  Logger(const std::string& path) {
    fd_ = ::open(path.c_str(), O_CREAT | O_APPEND | O_WRONLY | O_TRUNC, 0644);
    if (fd_ == -1) {
      throw std::system_error(errno, std::system_category(), "open");
    }
  }

  ~Logger() {
    if (fd_ != -1) {
      write_buf(buf, tmp_buf_size);
      ::close(fd_);
      fd_ = -1;
    }
  }

  void Write(const std::string& msg) {
    if (msg.size() + tmp_buf_size > BUFSIZE) {
      write_buf(buf, tmp_buf_size);
      tmp_buf_size = 0;
    }
    if (msg.size() > BUFSIZE) {
      write_buf(msg.data(), msg.size());
    } else {
      msg.copy(buf + tmp_buf_size, msg.size());
      tmp_buf_size += msg.size();
    }
  }

private:
  int fd_ = -1;
  size_t tmp_buf_size = 0;
  char* buf = new char[BUFSIZE];

  void write_buf(const char* data, size_t len) const {
    if (::write(fd_, data, len) !=
        static_cast<ssize_t>(len)) {
      throw std::system_error(errno, std::system_category(), "write");
    }
  }
};

BENCHMARK(BM_Logger)->Threads(1);

BENCHMARK_MAIN();
