#include <iostream>
#include <string>
#include <sys/stat.h>
#include <vector>

#include <gio/gdesktopappinfo.h>

bool file_exists(const std::string &filename) {
  struct stat buffer;
  return (stat(filename.c_str(), &buffer) == 0);
}

const std::string WHITESPACE = " \n\r\t\f\v";

inline std::string ltrim(const std::string &s) {
  size_t begin = s.find_first_not_of(WHITESPACE);
  return (begin == std::string::npos) ? "" : s.substr(begin);
}

inline std::string rtrim(const std::string &s) {
  size_t end = s.find_last_not_of(WHITESPACE);
  return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

inline std::string trim(const std::string &s) { return rtrim(ltrim(s)); }

static std::vector<std::string> search_prefix() {
  std::vector<std::string> prefixes = {""};

  auto xdg_data_dirs = std::getenv("XDG_DATA_DIRS");
  if (!xdg_data_dirs) {
    prefixes.emplace_back("/usr/share/");
    prefixes.emplace_back("/usr/local/share/");
  } else {
    std::string xdg_data_dirs_str(xdg_data_dirs);
    size_t start = 0, end = 0;

    do {
      end = xdg_data_dirs_str.find(':', start);
      auto p = xdg_data_dirs_str.substr(start, end - start);
      prefixes.push_back(trim(p) + "/");

      start = end == std::string::npos ? end : end + 1;
    } while (end != std::string::npos);
  }

  std::string home_dir = std::getenv("HOME");
  prefixes.push_back(home_dir + "/.local/share/");

  return prefixes;
}

std::string get_app_info_by_name(const std::string &app_id) {
  static std::vector<std::string> prefixes = search_prefix();

  std::vector<std::string> app_folders = {
      "", "applications/", "applications/kde/", "applications/org.kde."};

  std::vector<std::string> suffixes = {".desktop"};

  for (const auto &prefix : prefixes) {
    for (const auto &folder : app_folders) {
      for (const auto &suffix : suffixes) {
        auto filename = prefix + folder + app_id + suffix;

        if (file_exists(filename)) {
          return app_id + suffix;
        }
      }
    }
  }

  return "";
}

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << "error: no app id provided\n";
    return 1;
  }

  auto filename = get_app_info_by_name(std::string(argv[1]));
  GDesktopAppInfo *tmp_info = g_desktop_app_info_new(filename.c_str());
  if (tmp_info == NULL) {
    std::cerr << "error: desktop file with app id " << argv[1] << " not found\n";
    return 1;
  } else {
    std::cout << g_desktop_app_info_get_string(tmp_info, "Name") << "\n";
  }
  return 0;
}
