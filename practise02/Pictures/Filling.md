К фотографии Filling.png

1. Собственно дисплейный модуль, подключен к первой шине I2C, частота тактирования 400khz (Меньше он работать не может, поэтому, ему предоставлена такая роскошь, как сидеть на шине в одиночистве)
2. Модуль термометра, и измерения атмосферного давления, подключен ко второй шине I2C? Частота тактировния 100khz, такая частота выбрана, ибо на частоте 400khz, выдаются неверные данные температуры, несмотря на даташит
3. Модуль измерениия влажности воздуха, подключен ко второй шине I2c
4. Модуль SDCard, подключен по шине SPI, пока не задействован
5. Модуль часов реального времени, подключен ко второй шине i2c, пока не задействован
6. Датчик влажности почвы, подключен к АЦП (будь он неладен)
7. Сервоприводы sg90 для открытия окна
8. Wifi модуль, общение с МК происходит по средствам uart
9. Анализатор газа (углекислого), подключен к ацп (будь он неладен 2х)
10. Шинка usb
