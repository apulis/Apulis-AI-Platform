import { VariantType, useSnackbar } from 'notistack'


export type VariantTypeObj = {
  [variantType in VariantType]: string
}

const MESSAGE_TYPES: VariantTypeObj = {
  success: '成功',
  error: '错误',
  warning: '警告',
  default: '默认',
  info: '信息',
}

const { enqueueSnackbar } = useSnackbar()

const message = (variant: VariantType, message?: string) => () => {
  enqueueSnackbar(message || MESSAGE_TYPES[variant], { variant })
}



export default message