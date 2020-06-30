import React, {
  FunctionComponent,
  useCallback
} from 'react';
import {
  ListItem,
  ListItemText
} from '@material-ui/core';
import { useSnackbar } from 'notistack';
import copy from 'clipboard-copy';
import { useTranslation } from "react-i18next";

interface CopyableTextListItemProps {
  primary: string;
  secondary: string;
}

const CopyableTextListItem: FunctionComponent<CopyableTextListItemProps> = ({ primary, secondary }) => {
  const {t} = useTranslation();
  const { enqueueSnackbar } = useSnackbar();
  const onClick = useCallback(() => {
    copy(secondary).then(
      () => enqueueSnackbar(t('components.copyiedToClipboard'), { variant: 'success', autoHideDuration: 3000 }),
      () => enqueueSnackbar(t('components.failedToCopyText'), { variant: 'error', autoHideDuration: 3000 })
    )
  }, [secondary, enqueueSnackbar]);
  return (
    <ListItem button onClick={onClick}>
      <ListItemText primary={primary} secondary={secondary}/>
    </ListItem>
  );
}

export default CopyableTextListItem;
