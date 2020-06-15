import React, { useState, FC, useEffect } from "react";
import { TextField } from "@material-ui/core";
import './index.less';
import ExpandMoreIcon from '@material-ui/icons/ExpandMore';
import ChevronRightIcon from '@material-ui/icons/ChevronRight';
import TreeView from '@material-ui/lab/TreeView';
import TreeItem from '@material-ui/lab/TreeItem';
import FolderIcon from '@material-ui/icons/Folder';

const SelectTree: React.FC = () => {
  // const [expandedNodeIds, setExpandedNodeIds] = useState<string[]>([]);

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
  }

  return (
    <TextField
      select
      label="Input Path"
      fullWidth
      variant="filled"
    >
      <TreeView 
        defaultCollapseIcon={<><ExpandMoreIcon /><FolderIcon /></>}
        defaultExpandIcon={<><ChevronRightIcon /><FolderIcon /></>}
        onNodeToggle={onNodeToggle}
        onNodeSelect={onNodeSelect}
        className="selectTree"
      >
        <TreeItem nodeId="1" label="aaaaa" onLabelClick={onLabelClick}>
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
  ); 
};

export default SelectTree;
