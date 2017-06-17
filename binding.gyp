{
  "targets": [
    {
      "target_name": "SoundSensor",
      "sources": [ "src/Wrapper/SoundSensor.cpp","src/Wrapper/SoundWrapper.cpp",
      "src/Modules/AnalogModules/SoundSensor.cpp",
      "src/Libraries/ADS1015/ADS1015.cpp",
      "src/Libraries/Timer/AccurateTiming.cpp"
      ],
      "libraries": ["-l bcm2835","-l rt"]
    }
  ]
}
