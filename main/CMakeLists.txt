idf_component_register(SRCS "e-reader.c" EMBED_FILES "homepage.html" INCLUDE_DIRS ".")
spiffs_create_partition_image(storage ../books FLASH_IN_PROJECT)