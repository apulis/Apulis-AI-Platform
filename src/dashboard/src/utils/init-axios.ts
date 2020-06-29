import axios, {AxiosError, AxiosResponse, AxiosRequestConfig} from 'axios';
import { VariantType } from 'notistack';

import { initMessage } from './message'
export interface Message {
  (type: VariantType, msg: string): void;
}

let message: Message = (type: VariantType, msg: string) => {
  //
}

let userGroupPathCopy: string;

axios.defaults.baseURL = '/api';

axios.interceptors.request.use((ruquest: AxiosRequestConfig) => {
  // TODO: JWT auth
  return ruquest;
});

const codeMessage: any = {
  200: 'The server successfully returned the requested data',
  201: 'New or modified data was successful.',
  202: 'A request has been queued in the background (asynchronous task).',
  204: 'Data deleted successfully.',
  400: 'There was an error in the request, and the server did not create or modify data.',
  401: 'Does not have permission.',
  403: 'You are authorized, but access is prohibited.',
  404: 'The request was made for a record that does not exist, and the server did not operate',
  406: 'The requested type is not available.',
  410: 'The requested resource is permanently deleted and is no longer available.',
  422: 'When creating an object, a validation error occurred.',
  500: 'A server error occurred. Please check the server.',
  502: 'Gateway error.',
  503: 'Service is unavailable, server is temporarily overloaded or maintained。',
  504: 'Gateway timed out.',
};


axios.interceptors.response.use((response: AxiosResponse) => {
  return response;
}, (error: AxiosError) => {
  if (error.response) {
    const { status } = error.response;
    if (status === 400) {
      message('error', error.response.data.message || '');
    } else  if (status === 401) {
      message('error', codeMessage[status]);
      const redirect = window.location.href.split('?')[0];
      window.location.href = userGroupPathCopy + '/user/login' + '?redirect=' + encodeURIComponent(redirect);
    } else if (status === 403) {
      message('error', codeMessage[status]);
    } else if (status === 500) {
      message('error', codeMessage[status]);
    } else if (status === 502) {
      message('error', codeMessage[status]);
    } else {
      message('error', codeMessage[status]);
    }
  } else {
    message('error', 'unexpected error')
  }
  
  return Promise.reject(error)
});


const initAxios = (msg: Message, userGroupPath: string) => {
  message = msg;
  userGroupPathCopy = userGroupPath;
  initMessage(msg);
}

export const request = async (options: AxiosRequestConfig) => {
  return await axios(options)
}

export default initAxios;