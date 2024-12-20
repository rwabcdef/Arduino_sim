/*
 * System0.hpp
 *
 *  Created on: 13 Dec 2024
 *      Author: rw123
 */

#ifndef SYSTEM0_HPP_
#define SYSTEM0_HPP_

#include "env.hpp"
#include "uart_wrapper.hpp"
#include "Frame.hpp"
#include "Reader_config.hpp"
#include "Reader.hpp"
#include "Transport.hpp"

extern char readerRxBuffer[];
extern char readerAckBuffer[];
extern char writerTxBuffer[];

extern char writerTxFrameBuffer[];
extern char writerAckFrameBuffer[];

extern SerLink::Frame writerTxFrame;
extern SerLink::Frame writerAckFrame;

extern SerLink::Writer writer0;
extern SerLink::Reader reader0;

extern char transport0RxFrameBuffer[];
extern char transport0TxFrameBuffer[];

extern SerLink::Frame transport0RxFrame;
extern SerLink::Frame transport0TxFrame;

extern SerLink::Transport transport0;


#endif /* SYSTEM0_HPP_ */
