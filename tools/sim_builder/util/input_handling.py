

def get_yes_no(message):
  answer = ''
  while (answer != 'y' and answer != 'n'):
    answer = input(f'{message} (y/n): ').lower()

  return True if answer == 'y' else False

def get_int(message):
  answer = ''
  while (answer == ''):
    answer = input(f'{message} (integer): ').lower()
    try:
      answer = int(answer)
    except ValueError:
      answer = ''

  return answer

def get_str(message):
  return input(f'{message} (string): ')