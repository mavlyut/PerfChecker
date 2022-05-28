#include <benchmark/benchmark.h>

#include <system_error>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

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
      ::close(fd_);
      fd_ = -1;
    }
  }

  void Write(const std::string& msg) {
    if (::write(fd_, msg.data(), msg.size()) !=
        static_cast<ssize_t>(msg.size())) {
      throw std::system_error(errno, std::system_category(), "write");
    }
  }

private:
  int fd_ = -1;
};

static Logger TestLogger{"/tmp/benchmark_logger"};

void BM_Logger(benchmark::State& state) {
  for (auto _ : state) {
    TestLogger.Write("Test Message\n");
  }
}

BENCHMARK(BM_Logger)->Threads(1);

BENCHMARK_MAIN();
