#!/usr/bin/env python3
"""Validate rebuilt patch structure and footprint against the supplied original."""
import re
import struct
from pathlib import Path

ROOT=Path(__file__).resolve().parent
TARGETS = [('US',513,0x8135A19C),('EU',514,0x8135A244),('JP',512,0x8135965C)]

def parse(fn):
    blocks=[]
    for raw in Path(fn).read_text().splitlines():
        line=raw.strip()
        if not line: continue
        if line=='[GameCube Controller Support]': blocks.append({'pairs':[]}); continue
        if not blocks: continue
        if line.startswith(('minversion=','maxversion=','amount=')):
            k,v=line.split('=',1);blocks[-1][k]=int(v)
        elif line.startswith('offset='):
            blocks[-1]['pairs'].append([int(line.partition('=')[2],16),None])
        elif line.startswith('patch='):
            pairs=blocks[-1]['pairs']; assert pairs and pairs[-1][1] is None
            pairs[-1][1]=[int(s,16) for s in line.partition('=')[2].split(',')]
        else: raise AssertionError('Unknown line: '+line)
    return blocks

def bytes_at(block):
    result={}
    for offset,words in block['pairs']:
        assert words is not None and len(words)>0 and offset%4==0
        for i,w in enumerate(words):
            assert 0<=w<2**32
            addr=offset+i*4
            assert addr not in result,(hex(addr),'overlapping patches')
            result[addr]=w
    return result

original={b['minversion']:b for b in parse('/mnt/data/gcn.ini')}
updated={b['minversion']:b for b in parse(ROOT/'gcn.ini')}
assert sorted(original)==sorted(updated)==[512,513,514]
for r,ver,entry in TARGETS:
    old=original[ver];new=updated[ver]
    assert old['maxversion']==new['maxversion']==ver
    assert new['amount']==len(new['pairs'])
    old_bytes=bytes_at(old); new_bytes=bytes_at(new)
    assert set(new_bytes).issubset(old_bytes),f'{r}: unexpected newly allocated addresses: {set(new_bytes)-set(old_bytes)}'
    hooks=[o for o,w in new['pairs'] if len(w)==1 and 0x81330000<o<0x81340000]
    assert len(hooks)==4, (r, hooks)
    for hook_addr in hooks:
        instr=new_bytes[hook_addr]
        disp=(instr&0x03fffffc)
        if disp&0x02000000: disp-=0x04000000
        target=(hook_addr+disp)&0xffffffff
        assert instr&1==1 and target==entry,(r,hex(hook_addr),hex(target),hex(entry))
    elf=(ROOT/f'hook_{r}.elf').read_bytes()
    assert elf[:4]==b'\x7fELF' and elf[5]==2 and elf[18:20]==b'\x00\x14'
    print(f'{r} / v{ver}: OK, {len(new["pairs"])} entradas, {len(new_bytes)*4} bytes de patches, novos enderecos = 0, hooks = 4')
print('PASS: regions, patch amounts, nonoverlap, original-footprint and hook targets')
