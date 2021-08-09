#!/usr/bin/env python3

import requests
import sys
import argparse
import re

argp = argparse.ArgumentParser()
argp.add_argument('--oneline', action='store_true',
                  help='Make one-lined changelog entries (for 4.0 and older)')
argp.add_argument('pullrequest', metavar='PULL_REQUEST', nargs='+',
                  help='Make changelogs for these Pull Request #\'s')
arguments = argp.parse_args()

ticket_regex = re.compile(r'(?:[Cc]loses|[Ff]ixes)? #(\d+)')

out = ''
for pr in arguments.pullrequest:
    if pr[0] == '#':
        pr = pr[1:]
    try:
        res = requests.get('https://api.github.com/repos/PowerDNS/pdns/pulls/'
                           '{}'.format(pr))
        pr_info = res.json()
    except (requests.exceptions.HTTPError, ValueError) as e:
        print(e)
        sys.exit(1)

    if arguments.oneline:
        out += '- `#{pr} <{url}>`__: {title}'.format(
            pr=pr, url=pr_info['html_url'], title=pr_info['title']
        )
    else:
        out += '  .. change::\n' + \
               '  :tags: XXXXXX\n' + \
               '  :pullreq: {}\n'.format(pr)
        tickets = re.findall(ticket_regex, pr_info['body'])
        if len(tickets):
            out += '  :tickets: {}\n'.format(', '.join(tickets))
        out += '\n  {}'.format(pr_info['title'].capitalize())

    if pr_info['user']['login'].lower() not in ['ahupowerdns', 'habbie',
                                                'pieterlexis', 'rgacogne',
                                                'aerique']:
        try:
            user_info = requests.get(pr_info['user']['url']).json()
        except (requests.exceptions.HTTPError, ValueError) as e:
            print(e)
            sys.exit(1)
        out += ' ({})'.format(user_info['name'])
    out += '\n'

    if not arguments.oneline:
        out += '\n'

print(out)
