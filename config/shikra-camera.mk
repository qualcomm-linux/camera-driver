# SPDX-License-Identifier: GPL-2.0-only

CONFIG_SPECTRA_ISP := y
CONFIG_SPECTRA_OPE := y
CONFIG_SPECTRA_TFE := y
CONFIG_SPECTRA_SENSOR := y
CONFIG_ENABLE_US_API := y
CONFIG_SHIKRA := y

# Flags to pass into C preprocessor
ccflags-y += -DCONFIG_SPECTRA_ISP=1
ccflags-y += -DCONFIG_SPECTRA_OPE=1
ccflags-y += -DCONFIG_SPECTRA_TFE=1
ccflags-y += -DCONFIG_SPECTRA_SENSOR=1
ccflags-y += -DCONFIG_ENABLE_US_API=1
ccflags-y += -DCONFIG_SHIKRA=1

# External Dependencies
ifeq ($(CONFIG_QCOM_VA_MINIDUMP), y)
KBUILD_CPPFLAGS += -DCONFIG_QCOM_VA_MINIDUMP=1
endif
