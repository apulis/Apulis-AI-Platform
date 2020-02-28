import { VariantType } from 'notistack';
import { Message } from './init-axios';

let message: Message = (type: VariantType, msg: string) => {
  //
}

export const initMessage = (msg: Message) => {
  message = msg;
}


export default message;