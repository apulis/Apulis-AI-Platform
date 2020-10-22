

export const validateInteractivePorts = (val: string) => {
  if (val) {
    let flag = true;
    const arr = val.split(',');
    if (arr.length > 1) {
      arr.forEach(n => {
        const _n = Number(n)
        if (!_n || _n < 40000 || _n > 49999 || !Number.isInteger(_n)) flag = false;
      });
    } else {
      flag = Number(val) >= 40000 && Number(val) <= 49999 && Number.isInteger(Number(val));
    }
    return flag;
  }
  return true;
}