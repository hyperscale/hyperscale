workspace(name = "hyperscale")

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

git_repository(
    name = "com_github_nelhage_rules_boost",
    commit = "1e3a69bf2d5cd10c34b74f066054cd335d033d71",
    remote = "https://github.com/nelhage/rules_boost",
    shallow_since = "1591047380 -0700",
)

load("@com_github_nelhage_rules_boost//:boost/boost.bzl", "boost_deps")
boost_deps()


load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
http_archive(
    name = "hyperconsole",
    urls = ["https://github.com/hyperlib/console/archive/master.zip"],
    sha256 = "7359a69d5a35455c22eb2438f2ba958064d90ca20cc571c320ec14bfbb935867",
    strip_prefix = "console-master"
)

# see https://github.com/google/llvm-bazel/blob/http-archive-demo/http-archive-demo/WORKSPACE


# LLVM_COMMIT = "499bce3abab8a362b9b4197944bd40b826c736c4"
LLVM_COMMIT = "f482497c381cf9beee513864123cf975bd72babf"

LLVM_BAZEL_TAG = "llvm-project-%s" % (LLVM_COMMIT,)

#LLVM_BAZEL_SHA256 = "a05a83300b6b4d8b45c9ba48296c06217f3ea27ed06b7e698896b5a3b2ed498d"
LLVM_BAZEL_SHA256 = "359a9c4dc38799835255aaa5a331ef3ff1207362c87ba77ae65546b8dc09f09"

http_archive(
    name = "llvm-bazel",
    #sha256 = LLVM_BAZEL_SHA256,
    strip_prefix = "llvm-bazel-{tag}/llvm-bazel".format(tag = LLVM_BAZEL_TAG),
    url = "https://github.com/google/llvm-bazel/archive/{tag}.tar.gz".format(tag = LLVM_BAZEL_TAG),
)

#LLVM_SHA256 = "a154965dfeb2b5963acc2193bc334ce90b314acbe48430ba310d8a7c7a20de8b"

LLVM_SHA256 = "0b3b3d59abbc426e7d019ffa24f035821d2e50aa6efe79471f3c549ceb370411"

LLVM_URLS = [
    "https://storage.googleapis.com/mirror.tensorflow.org/github.com/llvm/llvm-project/archive/{commit}.tar.gz".format(commit = LLVM_COMMIT),
    "https://github.com/llvm/llvm-project/archive/{commit}.tar.gz".format(commit = LLVM_COMMIT),
]

http_archive(
    name = "llvm-project-raw",
    build_file_content = "#empty",
    sha256 = LLVM_SHA256,
    strip_prefix = "llvm-project-" + LLVM_COMMIT,
    urls = LLVM_URLS,
)

load("@llvm-bazel//:terminfo.bzl", "llvm_terminfo_disable")

llvm_terminfo_disable(
    name = "llvm_terminfo",
)

load("@llvm-bazel//:zlib.bzl", "llvm_zlib_disable")

llvm_zlib_disable(
    name = "llvm_zlib",
)

load("@llvm-bazel//:configure.bzl", "llvm_configure")

llvm_configure(
    name = "llvm-project",
    src_path = ".",
    src_workspace = "@llvm-project-raw//:WORKSPACE",
)















