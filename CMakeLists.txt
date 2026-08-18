cmake_minimum_required(VERSION 3.22)
project(TolibjonProductionVocal VERSION 1.0.0)

add_subdirectory(JUCE)

juce_add_plugin(TolibjonProduction
    COMPANY_NAME "TolibjonProduction"
    BUNDLE_ID "com.TolibjonProduction.VocalStrip"
    PLUGIN_NAME "Tolibjon Production Vocal Strip"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD FALSE
    PLUGIN_MANUFACTURER_CODE Tlbj
    PLUGIN_CODE Tlb1
    FORMATS VST3
    PRODUCT_NAME "Tolibjon Production Vocal Strip"
)

# Керакли JUCE Header файлини яратиш
juce_generate_juce_header(TolibjonProduction)

# Файлларни аниқ топиш (эски русча ёки янги инглизча папкадан бўлса ҳам)
file(GLOB PluginSources 
    "Source/*.cpp" "Source/*.h"
    "Источник/*.cpp" "Источник/*.h"
)

target_sources(TolibjonProduction PRIVATE ${PluginSources})

target_compile_definitions(TolibjonProduction
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
)

target_link_libraries(TolibjonProduction
    PRIVATE
        juce::juce_audio_utils
        juce::juce_dsp
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_warning_flags
)
