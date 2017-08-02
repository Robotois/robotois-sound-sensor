{
  "targets": [
    {
      "target_name": "SoundSensor",
      "sources": [
          "src/wrapper/SoundSensor.cpp",
          "src/wrapper/SoundWrapper.cpp",
          "src/SoundSensor.cpp",
          "src/libraries/robotois-ADS1015/ADS1015.cpp",
      ],
      "libraries": ["-l bcm2835","-l rt"]
    }
  ]
}
