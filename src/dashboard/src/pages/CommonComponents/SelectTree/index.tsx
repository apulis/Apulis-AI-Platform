import React, { useState, FC, useEffect, CSSProperties } from "react";
import { TextField } from "@material-ui/core";
import './index.less';
import ExpandMoreIcon from '@material-ui/icons/ExpandMore';
import ChevronRightIcon from '@material-ui/icons/ChevronRight';
import TreeView from '@material-ui/lab/TreeView';
import TreeItem from '@material-ui/lab/TreeItem';
import FolderIcon from '@material-ui/icons/Folder';

interface SelectTreeProps {
  label?: string;
  style?: CSSProperties | undefined;
  formObj: any;
  name: string;
}

const SelectTree = (props: SelectTreeProps) => {
  const { label, style, formObj, name } = props; 
  const { errors, register, setValue, clearError } = formObj;
  const [selectFolder, setSelectFolder] = useState('');

  const onNodeToggle = (e: any, nodeIds: any) => {
    e.stopPropagation();
  }

  const onLabelClick = (e: any) => {
    e.preventDefault()
    console.log('onLabelClick')
  }

  const onNodeSelect = (e: any, value: any) => {
    e.preventDefault();
    console.log('onNodeSelect', value)
    setSelectFolder(String(value));
    setValue(name, String(value));
    clearError(name);
  }

  const onIconClick = (e: any) => {
    console.log('onIconClick')
  }

  return (
    <div className="selectTreeWrap">
      <TextField
        label={label || ''}
        fullWidth
        variant="filled"
        value={selectFolder}
        className="inputField"
        name={name}
        error={Boolean(errors[name])}
        helperText={errors[name] ? errors[name].message : ''}
        InputLabelProps={{ shrink: true }}
        inputRef={register({
          required: `${label} is required！`
        })}
      />
      <TextField
        select
        fullWidth
        variant="filled"
        style={style}
      >
        <TreeView 
          defaultCollapseIcon={<><ExpandMoreIcon /><FolderIcon /></>}
          defaultExpandIcon={<><ChevronRightIcon /><FolderIcon /></>}
          onNodeToggle={onNodeToggle}
          onNodeSelect={onNodeSelect}
          className="selectTree"
        >
          <TreeItem nodeId="1" label="aaaaa" onLabelClick={onLabelClick} onIconClick={onIconClick}>
            <TreeItem nodeId="2" label="Calendar" onLabelClick={onLabelClick} />
            <TreeItem nodeId="3" label="Chrome" onLabelClick={onLabelClick} />
            <TreeItem nodeId="4" label="Webstorm" onLabelClick={onLabelClick} />
          </TreeItem>
          <TreeItem nodeId="5" label="Documents" onLabelClick={onLabelClick}>
            <TreeItem nodeId="10" label="OSS" onLabelClick={onLabelClick} />
            <TreeItem nodeId="6" label="Material-UI" onLabelClick={onLabelClick}>
              <TreeItem nodeId="7" label="src" onLabelClick={onLabelClick}>
                <TreeItem nodeId="8" label="index.js" onLabelClick={onLabelClick} />
                <TreeItem nodeId="9" label="tree-view.js" />
              </TreeItem>
            </TreeItem>
          </TreeItem>
        </TreeView>
      </TextField>
    </div>
  ); 
};

export default SelectTree;
