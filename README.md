# php-sdhumming

---
## Dependency (ubuntu)
	sudo apt-get install build-essential php5 php5-dev

## Install
	cd php-sdhumming/src
	phpize
	./configure
	make
	sudo make install

## Config (Required! Or else your PHP engine will fail to start)
	Edit php-sdhumming/conf/etc/php5/mods-available/sdhumming.ini
	Adjust the value of sdhumming.model and sdhumming.info

## Deploy
	cd php-sdhumming/conf
	sudo cp ./etc/php5/mods-available/sdhumming.ini /etc/php5/mods-available/sdhumming.ini 
	cd /etc/php5/cli/conf.d/
	sudo ln -s ../../mods-available/sdhumming.ini 50-sdhumming.ini 
	cd /etc/php5/apache2/conf.d/
	sudo ln -s ../../mods-available/sdhumming.ini 50-sdhumming.ini
	sudo service apache2 restart
