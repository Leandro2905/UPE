#####################################
# Generate the pcidata source files in the x86 build
#
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/pci_classes.c ${CMAKE_CURRENT_BINARY_DIR}/pci_classes.h
    COMMAND native-bin2c ${CMAKE_CURRENT_SOURCE_DIR}/legacy/bus/pci_classes.ids ${CMAKE_CURRENT_BINARY_DIR}/pci_classes.c ${CMAKE_CURRENT_BINARY_DIR}/pci_classes.h BINSTR ClassTable INIT_SECTION ${CMAKE_CURRENT_SOURCE_DIR}/include/hal.h
    DEPENDS native-bin2c ${CMAKE_CURRENT_SOURCE_DIR}/legacy/bus/pci_classes.ids)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/pci_vendors.c ${CMAKE_CURRENT_BINARY_DIR}/pci_vendors.h
    COMMAND native-bin2c ${CMAKE_CURRENT_SOURCE_DIR}/legacy/bus/pci_vendors.ids ${CMAKE_CURRENT_BINARY_DIR}/pci_vendors.c ${CMAKE_CURRENT_BINARY_DIR}/pci_vendors.h BINSTR VendorTable INIT_SECTION ${CMAKE_CURRENT_SOURCE_DIR}/include/hal.h
    DEPENDS native-bin2c ${CMAKE_CURRENT_SOURCE_DIR}/legacy/bus/pci_vendors.ids)
#####################################
