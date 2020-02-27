import React from 'react';
import { Card, Col, Row } from 'antd';
import { Link } from 'umi';


const Index: React.FC = () => {
  console.log('index')
  return (
    <>
      <Row>
        <Col span={8}>
          <Card size="small"
            title="用户" 
            extra={<Link to="/admin/user/add">创建用户</Link>} style={{ width: 300 }}
          >
            <h2>{2} 人</h2>
          </Card>
        </Col>
        <Col span={8}>
          <Card size="small"
            title="用户组" 
            extra={<Link to="/admin/group/add">创建用户组</Link>} style={{ width: 300 }}
          >
            <h2>{2} 组</h2>
          </Card>
        </Col>
        <Col span={8}>
          <Card size="small"
            title="自定义角色" 
            extra={<Link to="/admin/role/add">创建自定义角色</Link>} style={{ width: 300 }}
          >
            <h2>{2} 个</h2>
          </Card>
        </Col>
      </Row>
    </>
  )
}

export default Index;