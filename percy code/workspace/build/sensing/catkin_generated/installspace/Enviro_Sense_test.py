#!/usr/bin/python
# -*- coding:utf-8 -*-
import time
import SH1106 #OLED
import ICM20948 #Gyroscope/Acceleration/Magnetometer
import BME280   #Atmospheric Pressure/Temperature and humidity
import SI1145   #UV
import TSL2591  #LIGHT
#import ADS1015  #ad
import SGP40 #VOC
import rospy
from PIL import Image,ImageDraw,ImageFont
import math
import csv



bme280 = BME280.BME280()
bme280.get_calib_param()
light = TSL2591.TSL2591()
si1145 = SI1145.SI1145()
#ad = ADS1015.ADS1015()
sgp40 = SGP40.SGP40()
oled = SH1106.SH1106()
icm20948 = ICM20948.ICM20948()

class Environment:
	f = open('/home/gdp31/GDP31/workspace/src/sensing/src/envi_data.csv','w+') #csv file
	wc = csv.writer(f) #csv function
	image = Image.new('1', (oled.width, oled.height), "BLACK")
	draw = ImageDraw.Draw(image)
	x = 0
	font = ImageFont.truetype('/home/gdp31/GDP31/workspace/src/sensing/src/Font.ttc', 10)

	def timer_callback(self, boo):
		bme = []
		bme = bme280.readData()
		pressure = round(bme[0], 2) 
		temp = round(bme[1], 2) 
		hum = round(bme[2], 2)
		lux = round(light.Lux(), 2)
		uv = round(si1145.readdata()[0], 2) 
		ir = round(si1145.readdata()[1], 2)
		#voc = round(sgp40.readata(), 2)
		row_list = [pressure,temp,hum, lux, uv, ir]
		self.wc.writerow(row_list) #write data into csv

		self.draw.rectangle((0, 0, 128, 64), fill = 0)
		
		self.draw.text((0, 0), str(pressure), font = self.font, fill = 1)
		self.draw.text((40, 0), 'hPa', font = self.font, fill = 1)
		self.draw.text((0, 15), str(temp), font = self.font, fill = 1)
		self.draw.text((40, 15), 'C', font = self.font, fill = 1)
		self.draw.text((0, 30), str(hum), font = self.font, fill = 1)
		self.draw.text((40, 30), '%RH', font = self.font, fill = 1)
		
		self.draw.text((0, 45), str(lux), font = self.font, fill = 1)
		self.draw.text((40, 45), 'Lux', font = self.font, fill = 1)
		
		self.draw.text((65, 0), str(uv), font = self.font, fill = 1)
		self.draw.text((105, 0), 'UV', font = self.font, fill = 1)
		self.draw.text((65, 15), str(ir), font = self.font, fill = 1)
		self.draw.text((105, 15), 'IR', font = self.font, fill = 1)


		

if __name__ == '__main__':
       
    try:
	class_enviro = Environment()
	rospy.init_node('Enviro_module')
        class_enviro.timer = rospy.Timer(rospy.Duration(1), class_enviro.timer_callback)
        rospy.spin()
    except rospy.ROSInterruptException:
	class_enviro.f.close()
        pass
