#!/usr/bin/env bash
#
# Script file to compile all C++ source files in or under the
# current directory.  This has been used in the OpenSUSE and Ubuntu
# environments with the GCC and Clang compilers and linkers
CCOptions=''
while [[ $# -gt 0 && "${1}" = -* ]]; do  # of course options must not have spaces
  CCOptions="${CCOptions} ${1}"
  shift
done

executableFileName="${1:-project}"
versionID="23.12.26"
echo "${0##*/} version ${versionID}"


### Check GCC and Clang versions on Tuffix, and upgrade if needed - Usually a one-time occurrence
###  The procedure should be removed once Tuffix is configured out-of-the-box with correct versions
CheckVersion()
{
  #  See Parameter Expansion section of Bash man page for "%%"'s' Remove matching suffix pattern
  #  behavior (https://linux.die.net/man/1/bash)
  #
  #  ${parameter,,}     ==> lower case
  #  ${parameter^^}     ==> upper case
  #  ${parameter%word}  ==> Remove matching suffix pattern (shortest matching pattern)
  #  ${parameter%%word} ==> Remove matching suffix pattern (longest matching pattern)

  buffer=( $(g++ --version ) )
  gccVersion="${buffer[3]%%.*}"

  # This is pretty fragile, but version 10 and 12+ display the version slightly differently
  buffer=( $(clang++ --version ) )
  if   [[ ${buffer[1],,} = "version" ]]; then  clangVersion="${buffer[2]%%.*}"
  elif [[ ${buffer[2],,} = "version" ]]; then  clangVersion="${buffer[3]%%.*}"
  fi

  RequiredGccVersion=13
  RequiredClangVersion=17



  ## Check minimum compiler versions
  if [[ "${gccVersion,,}" -lt "${RequiredGccVersion,,}"  ||  "${clangVersion,,}" -lt "${RequiredClangVersion,,}" ]]; then
    ## Minimum compiler versions not found, let's see if we can install them
    echo -e "\nGCC version ${RequiredGccVersion} and Clang version ${RequiredClangVersion} are required, but you're using GCC version ${gccVersion} and Clang version ${clangVersion}"

    ## Get distribution
    Distribution="$(lsb_release -is)"


    if [[ "${Distribution,,}" = "ubuntu" ]]; then

      Release="$(lsb_release -sr)"

      ## Ubuntu 22.04 support
      if [[ "${Release,,}" = "22.04" ]]; then
        echo -e "\nWould like to upgrade now?  This may require a system reboot. (yes or no)"
        read shall_I_upgrade

        if [[ "${shall_I_upgrade,,}x" = "yesx"  ||  "${shall_I_upgrade,,}x" = "yx" ]]; then
          echo -e "\nUpgrading could be a long and extensive process.\n\n ****  Make sure you have backups of all your data!\n\n Are you really sure?"
          read shall_I_upgrade
          if [[ "${shall_I_upgrade,,}x" = "yesx"  ||  "${shall_I_upgrade,,}x" = "yx" ]]; then

            echo -e "Yes.  Okay, attempting to upgrade now.  The upgrade requires super user privileges and you may be prompted for your password.\n"

            if [[ "${EUID:-$(id -u)}" -eq 0 ]]; then SUDO=''; else SUDO='/usr/bin/sudo'; fi
            ${SUDO} /bin/bash -svx -- <<-EOF   # the "-" after the "<<" allows leading tabs (but not spaces), a quoted EOF would mean literal input, i.e., do not substitute parameters
				InstallGccAlternative()
				{
				  update-alternatives  --install /usr/bin/gcc gcc /usr/bin/gcc-\${1} \${1}  --slave /usr/bin/g++         g++         /usr/bin/g++-\${1}        \
				                                                                            --slave /usr/bin/gcc-ar      gcc-ar      /usr/bin/gcc-ar-\${1}     \
				                                                                            --slave /usr/bin/gcc-nm      gcc-nm      /usr/bin/gcc-nm-\${1}     \
				                                                                            --slave /usr/bin/gcc-ranlib  gcc-ranlib  /usr/bin/gcc-ranlib-\${1} \
				                                                                            --slave /usr/bin/gcov        gcov        /usr/bin/gcov-\${1}       \
				                                                                            --slave /usr/bin/gcov-dump   gcov-dump   /usr/bin/gcov-dump-\${1}  \
				                                                                            --slave /usr/bin/gcov-tool   gcov-tool   /usr/bin/gcov-tool-\${1}
				}


				InstallClangAlternative()
				{
				  update-alternatives  --install /usr/bin/clang clang /usr/bin/clang-\${1} \${1}  --slave /usr/bin/clang++            clang++            /usr/bin/clang++-\${1}              \
				                                                                                  --slave /usr/bin/clang-format       clang-format       /usr/bin/clang-format-\${1}         \
				                                                                                  --slave /usr/bin/clang-format-diff  clang-format-diff  /usr/bin/clang-format-diff-\${1}    \
				                                                                                  --slave /usr/bin/clang-tidy         clang-tidy         /usr/bin/clang-tidy-\${1}           \
				                                                                                  --slave /usr/bin/clang-tidy-diff    clang-tidy-diff    /usr/bin/clang-tidy-diff-\${1}.py
				}



				# Move gcc 11 and clang 13 to gcc ${RequiredGccVersion} and clang ${RequiredClangVersion} on Ubuntu 22.04 LTS

				# Someday, Ubuntu 22.04 standard packages will be updated to include the new versions, but for now ...
				add-apt-repository -y ppa:ubuntu-toolchain-r/test
				# add-apt-repository --remove ppa:ubuntu-toolchain-r/test/ppa

				apt -y update
				apt -y full-upgrade

				apt -y install build-essential manpages-dev gdb ddd                        # in case "Tuffix" is not installed (e.g. WSL)
				apt -y install gcc-${RequiredGccVersion} g++-${RequiredGccVersion}

				# apt -y install clang-${RequiredClangVersion} clang-tools-${RequiredClangVersion} clang-${RequiredClangVersion}-doc libclang-common-${RequiredClangVersion}-dev libclang-${RequiredClangVersion}-dev libclang1-${RequiredClangVersion} clang-format-${RequiredClangVersion} clang-tidy-${RequiredClangVersion} python3-clang-${RequiredClangVersion} clangd-${RequiredClangVersion}
				# apt -y install lldb-${RequiredClangVersion} lld-${RequiredClangVersion}
				# apt -y install libc++-${RequiredClangVersion}-dev libc++abi-${RequiredClangVersion}-dev
				wget https://apt.llvm.org/llvm.sh
				chmod +x llvm.sh
				./llvm.sh ${RequiredClangVersion} all
				rm llvm.sh

				apt -y autoremove



				InstallGccAlternative   "11"                         # build-essential default version
				InstallGccAlternative   "${RequiredGccVersion}"
				InstallClangAlternative "13"
				InstallClangAlternative "${RequiredClangVersion}"

				update-alternatives --auto gcc
				update-alternatives --auto clang

				###################
				# To remove:
				# sudo update-alternatives --set gcc   /usr/bin/gcc-9
				# sudo update-alternatives --set clang /usr/bin/clang-10

				###################
				# To select which version interactively:
				# sudo update-alternatives --config gcc
				# sudo update-alternatives --config clang

				###################
				# References:
				# https://apt.llvm.org/
				# https://stackoverflow.com/questions/67298443/when-gcc-11-will-appear-in-ubuntu-repositories/67406788#67406788

				EOF

          exit

          fi # upgrade? 2
        fi  # upgrade? 1

      else ## Ubuntu, but not 22.04
        echo -e "These required GCC and/or Clang versions are not available on Ubuntu ${Release}\n"
        echo -e "Please consider upgrading to Ubuntu 22.04 or better.  See https://youtu.be/2Mwo4BfJuvA"
        echo -e "    sudo do-release-upgrade"
      fi  ## Ubuntu 22.04 support

    fi

    echo -e "Build process aborted.  Please install GCC version ${RequiredGccVersion} or better and Clang version ${RequiredClangVersion} or better for your platform"
    echo -e "\n\nPlatform Information:\n====================="
    cat  /etc/*-release
    exit
  fi  # gccVersion || clangVersion
}


BuildWorkaroundHeader()
{
	cat - > "${complianceHelperFile_path}" <<-"EOF"   # the "-" after the "<<" allow leading tabs (but not spaces), the quoted EOF means literal input, i.e., do not substitute parameters
		/******************************************************************************
		** Auto generated from Build.sh
		******************************************************************************/
		// Paste contents of "Compliance_Workarounds.hpp" here
		#pragma once
		// Note: as of 29-OCT-2023, no workarounds are necessary once everyone has moved up to at least gcc version 13.2.1, ldd version 2.38, and clang version 17.0.3
	EOF
}




CheckVersion



### Vendors are releasing version of their C++ compilers and libraries with ever more C++23 compliant features, but
### they're not fully compliant yet.  And Linux vendors are slow to make these new versions available.  As a result,
### we see in practice various degrees of compliance which we can compensate for by providing the missing pieces in a
### header file that is added to each translation unit.  We search for such a header file first in the current working
### directory, then in the same directory as this build script, and if not found in either of those locations we create
### it on the fly.  This search order allows such a (potentially updated) header file to be provided with each project.
complianceHelperFile_filename="Compliance_Workarounds.hpp"

complianceHelperFile_path="./${complianceHelperFile_filename}"
if [[ ! -f "${complianceHelperFile_path}" ]]; then                                   # Is the helper file in the current directory?
  complianceHelperFile_path="${0%/*}/${complianceHelperFile_filename}"
  if [[ ! -f "${complianceHelperFile_path}" ]]; then                                 # Is the helper file in the same directory as this script (Build.sh)
    complianceHelperFile_path="$( mktemp -p /tmp ${complianceHelperFile_filename}.XXXXXXXX || exit 3 )"
    trap 'rm -f "${complianceHelperFile_path}"' EXIT                                 # clean up after myself
    BuildWorkaroundHeader
  fi
fi



# Find and display all the C++ source files to be compiled ...
# temporarily ignore spaces when globing words into file names
temp=$IFS
  IFS=$'\n'
  sourceFiles=( $(find -L ./ -path ./.\* -prune -o -name "*.cpp" -print) )              # create array of source files skipping hidden folders (folders that start with a dot)
IFS=$temp

echo "Compiling in \"$PWD\" ..."
for fileName in "${sourceFiles[@]}"; do
  echo "  $fileName"
done
echo ""


#define options
GccOptions="  -Wall -Wextra -pedantic           \
              -Wdelete-non-virtual-dtor         \
              -Wduplicated-branches             \
              -Wduplicated-cond                 \
              -Wextra-semi                      \
              -Wfloat-equal                     \
              -Winit-self                       \
              -Wlogical-op                      \
              -Wnoexcept                        \
              -Wshadow                          \
              -Wnon-virtual-dtor                \
              -Wold-style-cast                  \
              -Wstrict-null-sentinel            \
              -Wsuggest-override                \
              -Wswitch-default                  \
              -Wswitch-enum                     \
              -Woverloaded-virtual              \
              -Wuseless-cast                    "

#             -Wzero-as-null-pointer-constant"


ClangOptions=" -stdlib=libc++ -Weverything          \
               -Wno-comma                           \
               -Wno-unused-template                 \
               -Wno-sign-conversion                 \
               -Wno-exit-time-destructors           \
               -Wno-global-constructors             \
               -Wno-missing-prototypes              \
               -Wno-weak-vtables                    \
               -Wno-padded                          \
               -Wno-double-promotion                \
               -Wno-c++98-compat-pedantic           \
               -Wno-c++11-compat-pedantic           \
               -Wno-c++14-compat-pedantic           \
               -Wno-c++17-compat-pedantic           \
               -Wno-c++20-compat-pedantic           \
               -Wno-unsafe-buffer-usage             \
               -fdiagnostics-show-category=name     \
                                                    \
               -Wno-zero-as-null-pointer-constant   \
               -Wno-ctad-maybe-unsupported          "


CommonOptions="-g0 -O3 -DNDEBUG -pthread -std=c++23 -I./ -DUSING_TOMS_SUGGESTIONS -D__func__=__PRETTY_FUNCTION__"



ClangCommand="clang++ $CommonOptions $ClangOptions ${CCOptions}"
echo $ClangCommand -include \"${complianceHelperFile_path}\"
clang++ --version

if $ClangCommand -include "${complianceHelperFile_path}" -o "${executableFileName}_clang++"  "${sourceFiles[@]}"; then
  echo -e "\nSuccessfully created  \"${executableFileName}_clang++\""
else
  exit 1
fi

echo ""

GccCommand="g++ $CommonOptions $GccOptions ${CCOptions}"
echo $GccCommand -include \"${complianceHelperFile_path}\"
g++ --version

if $GccCommand -include "${complianceHelperFile_path}" -o "${executableFileName}_g++"  "${sourceFiles[@]}"; then
   echo -e "\nSuccessfully created  \"${executableFileName}_g++\""
else
   exit 1
fi
