import React from 'react';
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
      padding: '16px'
    },
    content: {
      padding: '16px'
    }
  })
);

interface Props {
  titleText?: string,
  timeText?: string,
  descText?: string,
  title: string,
  time: string,
  desc: string
}

const Description: React.FC<Props> = (props) => {
  const { titleText = '标题', timeText = '时间', descText = '描述' } = props
  const { title, time, desc } = props
  const styles = useStyles({})
  return (
    <table className={styles.table}>
      <tr>
        <th style={{ width: '20%' }} className={styles.label} colSpan={2}>{titleText}</th>
        <td style={{ width: '30%' }} className={styles.content} colSpan={3}>{title}</td>
        <th style={{ width: '20%' }} className={styles.label} colSpan={2}>{timeText}</th>
        <td style={{ width: '30%' }} className={styles.content} colSpan={3}>{time}</td>
      </tr>
      <tr>
        <th className={styles.label} colSpan={2}>{descText}</th>
        <td className={styles.content} colSpan={8}>{desc}</td>
      </tr>
    </table>
  )
}
export default Description;