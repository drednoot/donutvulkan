find_program(GLSLC NAMES glslc
             HINTS $ENV{VULKAN_SDK}/Bin
             REQUIRED)

function(add_spirv_shaders TARGET)
  set(shader_sources ${ARGN})

  set(OUT_DIR "${CMAKE_BINARY_DIR}/shaders")
  file(MAKE_DIRECTORY "${OUT_DIR}")

  foreach(src IN LISTS shader_sources)
    get_filename_component(name ${src} NAME)
    set(out "${OUT_DIR}/${name}.spv")

    # 3. Declare how to build each .spv from .glsl/.vert/.frag
    add_custom_command(
      OUTPUT "${out}"
      COMMAND ${CMAKE_COMMAND} -E make_directory "${OUT_DIR}"
      COMMAND ${GLSLC} "${CMAKE_CURRENT_SOURCE_DIR}/${src}" -o "${out}"
      DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${src}"
      COMMENT "Compiling shader ${src} → ${out}"
    )

    list(APPEND spirv_files "${out}")
  endforeach()

  target_sources(${TARGET} PRIVATE
    ${spirv_files}
  )
  set_source_files_properties(${spirv_files} PROPERTIES GENERATED TRUE)
endfunction()
