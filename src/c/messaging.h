#pragma once

//static void messaging_inbox_received(DictionaryIterator *, void *);
//static void messaging_inbox_dropped(AppMessageResult, void *);
void messaging_outbox_send(const char *);
void messaging_init(void);
void messaging_destroy(void);
