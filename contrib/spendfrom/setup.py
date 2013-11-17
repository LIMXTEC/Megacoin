from distutils.core import setup
setup(name='mgcspendfrom',
      version='1.0',
      description='Command-line utility for megacoin "coin control"',
      author='Gavin Andresen',
      author_email='gavin@megacoinfoundation.org',
      requires=['jsonrpc'],
      scripts=['spendfrom.py'],
      )
