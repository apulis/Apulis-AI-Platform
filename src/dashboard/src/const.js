
export const pollInterval = 3000;

export const NameReg = /^[A-Za-z0-9-_]+$/;

export const NoChineseReg = /^[^\u4e00-\u9fa5]+$/;

export const NoNumberReg = /^(?!.*[0-9])/;

export const NameErrorText = 'The Name Must be composed of letter, numbers, underscore or horizontal line！';

export const SameNameErrorText = 'Already has the same name!';

export const NoChineseErrorText = 'Cannot contain Chinese characters!';

export const OneInteractivePortsMsg = 'Must be a positive integer between 40000 and 49999！';

export const InteractivePortsMsg = 'Must be a positive integer between 40000 and 49999！Multiple can be separated by comma.';

export const NoNumberText = 'Cannot contain numbers';