#!/bin/bash

_build_mode=$1
_build_arch=$2
_project_path=$3
_project_name=$4
_build_args=$5

_gcc_version=$(gcc -dumpversion)

_dir_locals="(( nil . (\n\t
(flycheck-clang-language-standard . \"c++17\")\n\t
(projectile-enable-caching . t)\n\t
(projectile-project-name . \"$_project_name\")\n\t
(projectile-project-root . \"$_project_path/\")\n\t
)\n\t
)
(c-mode . ((mode . c++))) )\n\t"

echo -e $_dir_locals > $_project_path/.dir-locals.el
