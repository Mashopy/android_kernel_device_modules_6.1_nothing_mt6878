# SPDX-License-Identifier: GPL-2.0

# for the include dt-bindings header files in the kernel_device_modules-X.Y
# common, for include/dt-bindings
DTC_INCLUDE += $(DEVICE_MODULES_PATH)/include
# arm64
DTC_INCLUDE += $(DEVICE_MODULES_PATH)/arch/$(SRCARCH)/boot/dts $(DEVICE_MODULES_PATH)/arch/$(SRCARCH)/boot/dts/mediatek

# for cust.dtsi
#DTC_INCLUDE += $(DEVICE_MODULES_REL_PATH)/arch/$(SRCARCH)/boot/dts
DTC_INCLUDE += $(dir $(src))

# for devicetree overlay
DTC_FLAGS += -@

ifeq ($(CONFIG_ARCH_MT6878),y)
dtb-y += mt6878.dtb
dtbo-y += k6878v1_64.dtbo
endif

ifeq ($(CONFIG_ARCH_MT6897),y)
dtb-y += mt6897.dtb
dtbo-y += k6897v1_64.dtbo
endif

ifeq ($(CONFIG_ARCH_MT6985),y)
dtb-y += mt6985.dtb
dtbo-y += k6985v1_64_for_dx3.dtbo
endif

ifeq ($(CONFIG_ARCH_MT6989),y)
dtb-y += mt6989.dtb
dtbo-y += k6989v1_64.dtbo
endif
