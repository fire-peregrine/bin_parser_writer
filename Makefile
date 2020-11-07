# @file      Makefile
# @brief     Makefile
# @author    fire-peregrine
# @date      2020/10/24
# @copyright Copyright (C) fire-peregrine all rights reserved.
# @license   Released under the MIT License.

include Makefile.common

.PHONY: help build clean dump

# Show help message.
help:
	-@echo "*** $(SOFTWARE_NAME) $(SOFTWARE_VERSION) ***                "
	-@echo "                                                            "
	-@echo "    Commands :                                              "
	-@echo "        * build -> Build this library.                      "
	-@echo "        * clean -> Clean build environment.                 "
	-@echo "        * dump  -> Print internal variables (for debugging)."
	-@echo "                                                            "


# Build this software.
build:
	$(MAKE) -C build/


# Clean build environment.
clean:
	$(MAKE) -C build/ clean

# Dump internal variables.
dump:
	@echo "*** Variables *** "
	@echo "SOFTWARE_AUTHOR  : " $(SOFTWARE_AUTHOR)
	@echo "SOFTWARE_NAME    : " $(SOFTWARE_NAME)
	@echo "SOFTWARE_VERSION : " $(SOFTWARE_VERSION)
	@echo ""


