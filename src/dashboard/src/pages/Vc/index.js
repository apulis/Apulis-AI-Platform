import React from "react"
import ReactDOM from "react-dom"

export default class Vc extends React.Component {
  constructor() {
    super()
    this.state = {
      value: "wzy"
    }
  }
  handleChange(e) {
    this.setState({
      value: e.target.value
    })
  }

  render() {
    return (
      <div>
        <input value={this.state.value} onChange={this.handleChange.bind(this)} />
        <p>{this.state.value}</p>
      </div>
    )
  }
}