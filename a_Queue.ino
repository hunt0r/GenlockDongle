#define MEWPRO_BUFFER_LENGTH 64

byte queue[MEWPRO_BUFFER_LENGTH];
volatile int queueb = 0, queuee = 0;

void emptyQueue()
{
  queueb = queuee = 0;
  // empty serial receive buffer
  UCSR0B &= (~_BV(RXEN0)); // disable receiver
#ifndef UART_RECEIVER_DISABLE
  UCSR0B |= _BV(RXEN0); // enable receiver
#endif
}

boolean inputAvailable()
{
  if (queueb != queuee || Serial.available()) {
    return true;
  }
  return false;
}

byte myRead()
{
  if (queueb != queuee) {
    byte c;
    c = queue[queueb];
    queueb = (queueb + 1) % MEWPRO_BUFFER_LENGTH;
    return c;
  }
  return Serial.read();
}

// Utility functions
void queueIn(const char *p)
{
  int i;
  for (i = 0; p[i] != 0; i++) {
    queue[(queuee + i) % MEWPRO_BUFFER_LENGTH] = p[i];
  }
  queue[(queuee + i) % MEWPRO_BUFFER_LENGTH] = '\n';
  queuee = (queuee + i + 1) % MEWPRO_BUFFER_LENGTH;
}
