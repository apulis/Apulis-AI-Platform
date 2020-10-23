import React from 'react';
import { Divider } from '@material-ui/core';
import { createStyles, makeStyles, Theme } from '@material-ui/core/styles';

const useStyles = makeStyles((theme: Theme) =>
  createStyles({
    table: {
      margin: '16px 16px',
      width: '100%',
      border: 'solid 1px #f0f0f0',
    },
    row: {
      borderBottom: '1px solid #f0f0f0'
    },
    label: {
      padding: '16px',
      textAlign: 'left'
    },
    content: {
      padding: '16px',
      textAlign: 'left'
    }
  })
);

interface Props {
  titleText?: string,
  timeText?: string,
  descText?: string,
  title: string,
  time: string,
  desc: string,
  icon?: boolean,
  iconText?: string
}

const Description: React.FC<Props> = (props) => {
  const { titleText = '标题', timeText = '时间', descText = '描述' } = props
  const { title, time, desc, icon, iconText } = props
  const styles = useStyles({})
  return (
    <>
      <Divider />
      <table className={styles.table}>
        <tr>
          <th style={{ width: '15%' }} className={styles.label} colSpan={1}>
            {icon && <span style={{ border: '1px', backgroundColor: '#3f51b5',padding:'3px', color: 'white', marginRight:'5px' }}>{iconText}</span>}
            {titleText}
          </th>
          <td style={{ width: '35%' }} className={styles.content} colSpan={4}>{title}</td>
          <th style={{ width: '15%' }} className={styles.label} colSpan={1}>{timeText}</th>
          <td style={{ width: '35%' }} className={styles.content} colSpan={4}>{time}</td>
        </tr>
        <tr>
          <th className={styles.label} colSpan={1}>{descText}</th>
          <td className={styles.content} colSpan={9}>{desc}</td>
        </tr>
      </table>
    </>
  )
}
export default Description;