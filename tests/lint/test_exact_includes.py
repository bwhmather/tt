"""
Checks that all source and header files directly import the headers that they
need.
"""
import itertools
import pathlib
import sys

from tt_lint import (
    INCLUDE_ROOT,
    PROJECT_ROOT,
    SOURCE_ROOT,
    derive_include_from_path,
    enumerate_header_paths,
    enumerate_source_paths,
    header_path_for_source_path,
    read_ast_from_path,
    read_includes_from_path,
    resolve_clang_path,
    walk_file_preorder,
    is_source_path,
)

INCLUDE_ALIASES = {
    "asm-generic/errno-base.h": "errno.h",
    "asm-generic/ioctls.h": "sys/ioctl.h",
    "assert.h": "cassert",
    "bits/basic_string.h": "string",
    "bits/fcntl-linux.h": "fcntl.h",
    "bits/fs_ops.h": "filesystem",
    "bits/fs_path.h": "filesystem",
    "bits/getopt_core.h": "getopt.h",
    "bits/getopt_ext.h": "getopt.h",
    "bits/hashtable_policy.h": "unordered_map",
    "bits/mathcalls.h": "math.h",
    "bits/move.h": "utility",
    "bits/range_access.h": "ranges",
    "bits/resource.h": "sys/resource.h",
    "bits/shared_ptr_base.h": "memory",
    "bits/shared_ptr.h": "memory",
    "bits/sigaction.h": "signal.h",
    "bits/signum-generic.h": "signal.h",
    "bits/socket.h": "sys/socket.h",
    "bits/socket_type.h": "sys/socket.h",
    "bits/std_function.h": "functional",
    "bits/stdint-intn.h": "stdint.h",
    "bits/stdint-uintn.h": "stdint.h",
    "bits/stl_algo.h": "algorithm",
    "bits/stl_iterator.h": "iterator",
    "bits/stl_map.h": "map",
    "bits/stl_multimap.h": "map",
    "bits/stl_pair.h": "utility",
    "bits/stl_tree.h": "map",
    "bits/stl_vector.h": "vector",
    "bits/streambuf_iterator.h": "iterator",
    "bits/stringfwd.h": "string",
    "bits/struct_stat.h": "sys/socket.h",
    "bits/time.h": "time.h",
    "bits/types/clockid_t.h": "sys/types.h",
    "bits/types/FILE.h": "stdio.h",
    "bits/types/sigset_t.h": "sys/types.h",
    "bits/types/struct_timespec.h": "time.h",
    "bits/unique_ptr.h": "memory",
    "bits/unordered_map.h": "unordered_map",
    "bits/unordered_set.h": "unordered_set",
    "pngconf.h": "png.h",
}

BUILTIN_SYMBOLS = {
    "ssize_t": "sys/types.h",
    "pid_t": "sys/types.h",
}

IGNORED_SYMBOLS = {"NULL", "__u32", "std", "__normal_iterator"}


def test():
    passed = True

    for source_path in itertools.chain(
        enumerate_header_paths(),
        enumerate_source_paths(),
    ):
        if source_path.suffix == ".tpp":
            continue

        includes = set(read_includes_from_path(source_path))

        unused = set(includes)
        unused.discard("config.h")
        if source_path.is_relative_to(INCLUDE_ROOT) and source_path.relative_to(
            INCLUDE_ROOT
        ) == pathlib.Path("input/cursor.h"):
            unused.discard("linux/input-event-codes.h")

        indirect = dict()

        source = read_ast_from_path(source_path)

        for node in walk_file_preorder(source):
            if not node.referenced:
                continue
            ref = node.referenced
            if ref.spelling in BUILTIN_SYMBOLS:
                ref_include = BUILTIN_SYMBOLS[ref.spelling]
            else:
                if ref.location.file is None:
                    continue
                ref_path = resolve_clang_path(ref.location.file.name)

                if ref.spelling in IGNORED_SYMBOLS:
                    continue

                if ref_path == resolve_clang_path(source.spelling):
                    continue

                ref_include = derive_include_from_path(ref_path)

            ref_include = INCLUDE_ALIASES.get(ref_include, ref_include)
            if ref_include in includes:
                unused.discard(ref_include)
                continue

            indirect.setdefault(ref_include, set()).add(ref.spelling)

        missing_primary = None
        if is_source_path(source_path):
            header_path = header_path_for_source_path(source_path)
            if header_path is not None:
                ref_include = derive_include_from_path(header_path)
                unused.discard(ref_include)
                if ref_include not in includes:
                    missing_primary = ref_include

        if indirect or unused or missing_primary:
            msg = "======================================================================\n"
            msg += f"FAIL: test_exact_includes: {source_path.relative_to(PROJECT_ROOT)}\n"
            msg += "----------------------------------------------------------------------\n"

            msg += "Includes do not match requirements.\n\n"

            if missing_primary:
                msg += "The following primary include was missing:\n"
                msg += f"  - {missing_primary}\n"
                msg += "\n"

            if indirect:
                msg += "The following files were depended on indirectly:\n"
                for indirect_path, indirect_refs in sorted(indirect.items()):
                    msg += f"  - {indirect_path} ({', '.join(indirect_refs)})\n"
                msg += "\n"

            if unused:
                msg += "The following includes were unused:\n"
                for unused_path in sorted(unused):
                    msg += f"  - {unused_path}\n"
                msg += "\n"

            print(msg, file=sys.stderr)
            passed = False

    return passed


if __name__ == "__main__":
    sys.exit(0 if test() else 1)
