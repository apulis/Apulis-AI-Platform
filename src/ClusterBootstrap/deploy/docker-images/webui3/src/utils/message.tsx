import { VariantType } from 'notistack';
import { Message } from './init-axios';

let m: Message = (type: VariantType, msg: string) => {}

let message: Message = (type: VariantType, msg: string) => {
  m(type, msg);
}

let inited: boolean = false;

export const initMessage = (msg: Message) => {
  if (!inited) {
    inited = true;
    m = msg;
  }
}


export default message;