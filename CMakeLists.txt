cmake_minimum_required(VERSION 3.15)

project(AUDIO_PLUGIN_EXAMPLE VERSION 0.0.1)

if (DEFINED ENV{JUCE_DIR})
    message(STATUS "JUCE directory: $ENV{JUCE_DIR}")
    add_subdirectory($ENV{JUCE_DIR} "./JUCEbuild" EXCLUDE_FROM_ALL)
else()
    message(STATUS "Error: JUCE_DIR environment variable not defined, please set it with the correct Juce path")
endif()


juce_add_plugin(AudioPluginExample
# VERSION ...                               # Set this if the plugin version is different to the project version
# ICON_BIG ...                              # ICON_* arguments specify a path to an image file to use as an icon for the Standalone
# ICON_SMALL ...
# COMPANY_NAME ...                          # Specify the name of the plugin's author
# IS_SYNTH TRUE/FALSE                       # Is this a synth or an effect?
# NEEDS_MIDI_INPUT TRUE/FALSE               # Does the plugin need midi input?
# NEEDS_MIDI_OUTPUT TRUE/FALSE              # Does the plugin need midi output?
# IS_MIDI_EFFECT TRUE/FALSE                 # Is this plugin a MIDI effect?
# EDITOR_WANTS_KEYBOARD_FOCUS TRUE/FALSE    # Does the editor need keyboard focus?
# COPY_PLUGIN_AFTER_BUILD TRUE/FALSE        # Should the plugin be installed to a default location after building?
PLUGIN_MANUFACTURER_CODE Juce               # A four-character manufacturer id with at least one upper-case character
PLUGIN_CODE Dem0                            # A unique four-character plugin id with exactly one upper-case character
# GarageBand 10.3 requires the first letter to be upper-case, and the remaining letters to be lower-case
FORMATS AU VST3 Standalone                  # The formats to build. Other valid formats are: AAX Unity VST AU AUv3
PRODUCT_NAME "Audio Plugin Example")        # The name of the final executable, which can differ from the target name

juce_generate_juce_header(AudioPluginExample)

if (WIN32 AND DEFINED ENV{ASIO_SDK})
    message(STATUS "USING ASIO: ${ASIO_SDK}")
    set(USE_ASIO 1)
    target_include_directories(AudioPluginExample PUBLIC $ENV{ASIO_SDK})
else()
    set(USE_ASIO 0)
endif()

target_sources(AudioPluginExample
PRIVATE
src/components/Knob.cpp
src/PluginEditor.cpp
src/PluginProcessor.cpp)

target_compile_definitions(AudioPluginExample
PUBLIC
# JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
JUCE_VST3_CAN_REPLACE_VST2=0
JUCE_ASIO=${USE_ASIO})

target_link_libraries(AudioPluginExample
PRIVATE
# AudioPluginData           # If we'd created a binary data target, we'd link to it here
juce::juce_audio_utils
PUBLIC
juce::juce_recommended_config_flags
juce::juce_recommended_lto_flags
juce::juce_recommended_warning_flags)
