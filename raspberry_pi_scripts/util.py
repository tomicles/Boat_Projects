
def dataWriter(file, data, prefix=''):
    with open('{prefix}/{file}.out'.format(prefix=prefix, file=file), 'w') as f:
        f.write('{data}'.format(data=data))
