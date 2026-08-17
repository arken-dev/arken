-- Worker script executed for each task spawned by the examples in this
-- folder. Prints when it starts and finishes, and sleeps briefly to
-- simulate real work, so the printed order shows the queue behaviour.

return function(node, params)
  print(string.format('  [worker] start  key=%s i=%s', tostring(params.key), tostring(params.i)))
  os.sleep(params.sleep or 0.3)
  print(string.format('  [worker] finish key=%s i=%s', tostring(params.key), tostring(params.i)))
end
