import React, { useState } from 'react'
import { createPortal } from 'react-dom'
import Snackbar from '@material-ui/core/Snackbar'
import Alert from '@material-ui/lab/Alert'


type MessageProps = {
  type: 'success' | 'error' | 'warning' | 'info',
  message?: string,
}

const Message = (props: MessageProps) => {
  const [open, setOpen] = useState(true)
  const handleClose = (e: React.SyntheticEvent | React.MouseEvent, reason: string) => {

  }
  return createPortal(
    <Snackbar open={open} autoHideDuration={3000} onClose={handleClose}>
      <Alert severity={props.type}>
        {props.message}
      </Alert>
    </Snackbar>, document.body
  )
}

const message = function(type: MessageProps["type"], message: MessageProps['message']) {
  Message({
    type,
    message,
  })
}

export default message