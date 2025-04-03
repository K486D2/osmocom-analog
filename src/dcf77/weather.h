
uint16_t tx_weather(dcf77_tx_t *tx, time_t timestamp, int local_minute, int local_hour, int zone);
void rx_weather_reset(dcf77_rx_t *rx);
void rx_weather(dcf77_rx_t *rx, int minute, int hour, int zone, uint64_t frame);

