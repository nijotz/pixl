* Arduino memory limits are being hit. Look into replacing FastLED color arrays
  with code that will calculate color based on mapping and visualizations on
  the fly.
* Sending the same colors to multiple strips or strip segments is being done in
  Triangle because of last minute hackiness.
