package deviceplugin


func MaxUIntSlice(v []uint) uint {
	m := uint(0)
	for i, e := range v {
		if i==0 || e > m {
			m = e
		}
	}

	return m
}

func MinUIntSlice(v []uint) uint {
	m := uint(0)
	for i, e := range v {
		if i==0 || e < m {
			m = e
		}
	}

	return m
}
