
GAME:getViewport():setup(800, 600)

GAME:registerChapter(0, function(n)
  print("Welcome to chapter 0")

  -- Register objects by creating them and giving them a name
  -- chapter() also returns the created objects so you can directly pass it
  -- around in a lua variable. Here we use get() instead for demonstration
  -- purposes

  chapter("Image", "bg_grey_fading", "/media/$res/grey_fading.jpg")
  start_scene = chapter("Scene", "start")

  get("start"):setBackground(get("bg_grey_fading"))
  -- Same as:
  -- start_scene:setBackground(get("coke"))

  chapter("Image", "img_coke", "/media/$res/coke.jpg")

  GAME:goToScene(get("start"))

end)

GAME:runChapter(0)

