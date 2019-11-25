import React from "react"
import ReactDOM from "react-dom"
import axios from 'axios';

export default class Vc extends React.Component {
  constructor() {
    super()
    this.state = {
      value: "wzy"
    }
  }

  componentWillMount() {
    axios.get('/api/qjydev/listVc')
      .then((res) => {
        console.log(res)
      }, () => { })
  }

  handleChange(e) {
    this.setState({
      value: e.target.value
    })
  }

  add = () => {
    let vcName = 'test',
      quota = 'x',
      metadata = 'xx';
    axios.get(`/api/qjydev/addVc/${vcName}/${quota}/${metadata}`)
      .then((res) => {
        console.log(res)
      }, () => { })
  }

  update = () => {
    let vcName = 'test',
      quota = 'x',
      metadata = 'xxxxx';
    axios.get(`/api/qjydev/updateVc/${vcName}/${quota}/${metadata}`)
      .then((res) => {
        console.log(res)
      }, () => { })
  }

  delete = () => {
    let vcName = 'test';
    axios.get(`/api/qjydev/deleteVc/${vcName}`)
      .then((res) => {
        console.log(res)
      }, () => { })
  }

  render() {
    return (
      <div>
        <input value={this.state.value} onChange={this.handleChange.bind(this)} />
        <p>{this.state.value}</p>
        <div onClick={this.add}>add</div>
        <div onClick={this.update}>update</div>
        <div onClick={this.delete}>delete</div>
      </div>
    )
  }
}