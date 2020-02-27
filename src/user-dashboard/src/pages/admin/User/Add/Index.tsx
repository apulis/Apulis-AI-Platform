import React from 'react';
import { Form } from '@ant-design/compatible';
import { Input, Button, Col, Row } from 'antd';
import { FormComponentProps } from '@ant-design/compatible/es/form';

import styles from './index.less';

const FormItem = Form.Item;


const Add: React.FC<FormComponentProps> = props => {
  const { form: {getFieldDecorator, validateFields } } = props;
  const formItemLayout = {
    labelCol: {
      xs: { span: 21 },
      sm: { span: 21 },
    },
    wrapperCol: {
      xs: { span: 21 },
      sm: { span: 21 },
    },
  };
  const submit = () => {
    validateFields((err, result) => {
      if (!err) {
        console.log('result', result);
      }
    })
  };
  const removeUser = () => {

  }
  return (
    <div className={styles.add}>
      <Row>
        <Col span={4}>
          昵称 *
        </Col>
        <Col span={4}>
          用户名 *
        </Col>
        <Col span={4}>
          手机
        </Col>
        <Col span={4}>
          邮箱
        </Col>
        <Col span={4}>
          备注
        </Col>
      </Row>
      <Row>
        <Col span={4}>
          <FormItem { ...formItemLayout }>
            {getFieldDecorator('nickName', {
              rules: [{ required: true, message: '需要创建昵称'}],
            })(<Input placeholder="请输入昵称" />)}
          </FormItem>
        </Col>
        <Col span={4}>
          <FormItem { ...formItemLayout }>
            {getFieldDecorator('userName', {
              rules: [{ required: true, message: '需要创建用户名'}],
            })(<Input placeholder="请输入用户名" />)}
          </FormItem>
        </Col>
        <Col span={4}>
          <FormItem { ...formItemLayout }>
            {getFieldDecorator('phone')(<Input placeholder="请输入手机" />)}
          </FormItem>
        </Col>
        <Col span={4}>
          <FormItem { ...formItemLayout }>
            {getFieldDecorator('email')(<Input placeholder="请输入邮箱" />)}
          </FormItem>
        </Col>
        <Col span={4}>
          <FormItem { ...formItemLayout }>
            {getFieldDecorator('note')(<Input placeholder="请输入备注" />)}
          </FormItem>
        </Col>
        <Col span={2}><a onClick={() => removeUser() }>删除</a></Col>
        
      </Row>
      <Button onClick={submit}>新增用户</Button><span>每次最多创建 10 个用户</span>
      <Button type="primary">下一步</Button>
    </div>
    
  );
};

export default Form.create<FormComponentProps>()(Add)