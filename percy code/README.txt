To calbirate colour for red camera:

1) open terminal
2) type: source /home/gdp31/GDP31/workspace/devel/setup.bash
3) roslaunch delta_coords test.launch (don't have arduino connected when running this)

4) if it doesn't run and shuts down try v4l2-ctl --list-devices and make sure the USB 2.0 Camera (usb-70090000.xusb-2.3) is at /dev/video2. If not go to /home/gdp31/GDP31/workspace/src/delta_coords/launch/test.launch and change the /dev/video2 to /videoX for whatever X is in that command. It also may not work if you have all 4 usb hub slots plugged in.

5) hold red plant in middle so both plants can see it
6) print screen of all camera views and hsv masks
7) open this in gimp - go into terminal and type gimp
8) press 'o' then shift click on the colour you want, convert the pop up to HSV not RGB from dropdown menu
9) note down the HSV values, half the recorded hue value, convert the S and V values to a 255 range from the percentage given
10) within /home/gdp31/GDP31/workspace/src/delta_coords/scripts delta_coords_req.py and --_side.py are the scripts for finding the colour. !!make a copied back up of these files before editing!! right click and open in sublime text.
11) the cv2.inRange function on lines 73 or 82 governs the colour searched for. Make edits to this range with the range you have just gathered. delta_coords_req.py has two ranges. this is because red crosses the upper and lower limit of hue.
12) repeat from the top and see if the hsv mask is any better.
