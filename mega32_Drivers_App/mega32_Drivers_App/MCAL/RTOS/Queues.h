/*
 * Queues.h
 *
 * Created: 07/02/2016 12:02:42 م
 *  Author: hossam
 */ 


#ifndef QUEUES_H_
#define QUEUES_H_
void QueueCreate(int Queue[],int QueueSize);
void QueuePost(int Data);
int QueuePend(int *DataPtr);


#endif /* QUEUES_H_ */