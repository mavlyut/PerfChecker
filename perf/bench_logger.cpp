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
      write_buf(buf);
      ::close(fd_);
      fd_ = -1;
    }
  }

  void Write(const std::string& msg) {
    if (msg.size() + buf.size() > BUFSIZE) {
      write_buf(buf);
      buf = "";
    }
    if (msg.size() > BUFSIZE) {
      write_buf(msg);
    } else {
      buf += msg;
    }
  }

private:
  int fd_ = -1;
  std::string buf;

  void write_buf(std::string const& str) const {
    if (::write(fd_, str.data(), str.size()) !=
        static_cast<ssize_t>(str.size())) {
      throw std::system_error(errno, std::system_category(), "write");
    }
  }
};

static Logger TestLogger{"/tmp/benchmark_logger"};

void BM_Logger(benchmark::State& state) {
  for (auto _ : state) {
    TestLogger.Write("Test Message\n");
  }
}

BENCHMARK(BM_Logger)->Threads(1);

BENCHMARK_MAIN();
