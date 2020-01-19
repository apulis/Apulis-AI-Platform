import React from "react";
import { RouteComponentProps } from "react-router-dom"
import { Box, Grid, Paper, Typography, Button, CircularProgress, TextField } from "@material-ui/core";
import { createStyles, makeStyles } from "@material-ui/core/styles";
// import useFetch from "use-http";

import image1 from "../SignIn/image1.jpeg";
import image2 from "../SignIn/image2.jpeg";
import image3 from "../SignIn/image3.jpeg";

import UserContext from "../../contexts/User";

const useStyles = makeStyles(theme => createStyles({
  container: {
    backgroundImage: `url(${[image1, image2, image3][Date.now() % 3]})`,
    backgroundSize: "cover",
    backgroundPosition: "right"
  },
  form: {
    width: '100%', // Fix IE 11 issue.
    marginTop: theme.spacing(1),
  },
  submit: {
    margin: theme.spacing(3, 0, 2),
  },
}));

const SignUp: React.FC<RouteComponentProps> = ({ history }) => {
  const user = React.useContext(UserContext);
  const defaultNickName = (user && user.nickName) || ''
  let defaultUserName = ''
  if (user && user.group === 'Microsoft' && user.openId) {
    defaultUserName = user.openId.split('@', 1)[0]
  }
  const [signUp, setSignUp] = React.useState(false);
  const [nickName, setNickName] = React.useState(defaultNickName);
  const [userName, setUserName] = React.useState(defaultUserName);
  const [password, setPassword] = React.useState('');

  React.useEffect(() => {
    if (user.openId === undefined || user.group === undefined) {
      history.replace('/');
    }
    if (user.userName !== undefined) {
      history.replace('/');
    }
  }, [user, history])

  const styles = useStyles();

  const handleChange = React.useCallback(
    (event: React.ChangeEvent<HTMLInputElement>) => {
      if (event.target.name === 'nickName') {
        setNickName(event.target.value);
      } else if (event.target.name === 'password') {
        setPassword(event.target.value);
      } else if (event.target.name === 'userName') {
        setUserName(event.target.value);
      }
    },
    [setNickName, setUserName, setPassword]
  )
  const onButtonClick = () => {
    if(nickName.length < 1){
      alert('Invalid nickName!')
      return
    }
    if (!/^[a-zA-Z]{1}([a-zA-Z0-9]|[._]){2,20}$/.test(userName)) {
      alert('Invalid userName!')
      return
    }
    if(password.length < 6){
      alert('Invalid password!')
      return
    }

    setSignUp(true);
    const params = new URLSearchParams({
      nickName: nickName,
      userName: userName,
      password: password
    })
    fetch(`api/authenticate/signup?${params}`)
      .then(async (res: any) => {
        setSignUp(false);
        const data = await res.json();
        if (data.result) {
          // history.push('/')
          // reload api/bootstrap.js
          window.location.reload()
        } else {
          alert(data.error)
        }
      })
  }

  return (
    <Grid container justify="flex-end" className={styles.container}>
      <Grid
        item xl={4} lg={5} md={6} sm={8} xs={12} zeroMinWidth
        container alignItems="stretch" justify="flex-end"
      >
        <Paper square elevation={6}>
          <Box paddingTop={10} paddingRight={5} paddingBottom={10} paddingLeft={5} minHeight="100%" display="flex">
            <Grid container direction="column" spacing={10} alignItems="center" justify="space-between">
              <Grid item>
                <Typography variant="h2" component="h1" align="center">
                  依瞳 AI 平台
                </Typography>
              </Grid>
              <Grid item>
                <form className={styles.form} noValidate>
                  <TextField
                    variant="outlined"
                    margin="normal"
                    required
                    fullWidth
                    id="nickName"
                    label="NickName"
                    name="nickName"
                    autoFocus
                    defaultValue={nickName}
                    onChange={handleChange}
                  />
                  <TextField
                    variant="outlined"
                    margin="normal"
                    required
                    fullWidth
                    id="userName"
                    label="SSH UserName"
                    name="userName"
                    helperText="At least 3 characters"
                    defaultValue={userName}
                    onChange={handleChange}
                  />
                  <TextField
                    variant="outlined"
                    margin="normal"
                    required
                    fullWidth
                    name="password"
                    label="SSH Password"
                    type="password"
                    id="password"
                    helperText="At least 6 characters"
                    defaultValue={password}
                    onChange={handleChange}
                  />
                  <Button
                    fullWidth
                    variant="contained"
                    color="primary"
                    disabled={signUp}
                    className={styles.submit}
                    onClick={onButtonClick}
                  >
                    {signUp ? <CircularProgress size={24} /> : 'Sign up for Qjianjingyaun'}
                  </Button>
                </form>
              </Grid>

              <Grid item>
                <Typography variant="body2">
                  {"Built with "}
                  <span role="img" aria-label="heart">❤️</span>
                  {" by Qjianjingyaun"}
                </Typography>
              </Grid>
            </Grid>
          </Box>
        </Paper>
      </Grid>
    </Grid>
  );
};

export default SignUp;
