{
  "link_settings": {
    "libraries": ["-lwiringPi"]
  },
  "targets": [
    {
      "target_name": "hx711",
      "cflags!": ["-fno-exceptions"],
      "cflags_cc!": ["-fno-exceptions"],
      "ldflags": ["-lwiringPi"],
      "sources": [
        "source/binding.cpp",
        "source/hx711.cpp"
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")"
      ],
      "dependencies": [
        "<!(node -p \"require('node-addon-api').gyp\")"
      ],
      "defines": [
        "NAPI_DISABLE_CPP_EXCEPTIONS"
      ]
    }
  ]
}
