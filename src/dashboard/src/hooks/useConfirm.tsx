import React, {
  FunctionComponent,
  createContext,
  useCallback,
  useContext,
  useState
} from 'react';
import {
  Button,
  Dialog,
  DialogTitle,
  DialogContent,
  DialogContentText,
  DialogActions
} from '@material-ui/core';
import { useTranslation } from "react-i18next";


interface ConfirmContext {
  setMessage: (message: string) => void;
  setOpen: (open: boolean) => void;
  setResolve: (resolve: (value: boolean) => void) => void;
}

const ConfirmContext = createContext<ConfirmContext>({
  setMessage () { return; },
  setOpen () { return; },
  setResolve () { return; }
});

const ConfirmProvider: FunctionComponent = ({ children }) => {
  const {t} = useTranslation()
  const [message, setMessage] = useState<string>();
  const [open, setOpen] = useState(false);
  const [resolve, setResolve] = useState<(value: boolean) => void>();

  const onNoClick = useCallback(() => {
    setOpen(false);
    if (resolve) resolve(false);
  }, [resolve]);
  const onClose = onNoClick;
  const onYesClick = useCallback(() => {
    setOpen(false);
    if (resolve) resolve(true);
  }, [resolve]);

  return (
    <ConfirmContext.Provider value={{ setMessage, setOpen, setResolve }}>
      <>
        {children}
        <Dialog open={open} onClose={onClose}>
  <DialogTitle>{t('tips.deepLearningPlatform')}</DialogTitle>
          <DialogContent>
            <DialogContentText>{message}</DialogContentText>
          </DialogContent>
          <DialogActions>
  <Button autoFocus color="primary" onClick={onNoClick}>{t('hooks.no')}</Button>
            <Button onClick={onYesClick}>{t('hooks.yes')}</Button>
          </DialogActions>
        </Dialog>
      </>
    </ConfirmContext.Provider>
  );
};

const useConfirm = () => {
  const { setMessage, setOpen, setResolve } = useContext(ConfirmContext);
  const confirm = useCallback((message: string) => {
    setMessage(message);
    setOpen(true);
    return new Promise<boolean>((resolve) => {
      setResolve(() => resolve); // To avoid callbackify set-action
    });
  }, [setMessage, setOpen, setResolve])
  return confirm;
}

export { ConfirmProvider };
export default useConfirm;
