-- Worker script executed for each task spawned by the examples in this
-- folder. Prints when it starts and finishes, and sleeps briefly to
-- simulate real work, so the printed order shows the queue behaviour.

return function(node, params)
  print(string.format('  [worker] start  i=%s', tostring(params.i)))
  os.sleep(params.sleep or 0.3)
  print(string.format('  [worker] finish i=%s', tostring(params.i)))
end
