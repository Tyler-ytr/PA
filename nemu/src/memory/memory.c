#include "nemu.h"
#include "device/mmio.h"

#define PMEM_SIZE (128 * 1024 * 1024)

#define pmem_rw(addr, type) *(type *)({\
    Assert(addr < PMEM_SIZE, "physical address(0x%08x) is out of bound", addr); \
    guest_to_host(addr); \
    })

uint8_t pmem[PMEM_SIZE];
paddr_t page_translate(vaddr_t vaddr);

/* Memory accessing interfaces */

uint32_t paddr_read(paddr_t addr, int len) {
	int tempa=is_mmio(addr);
	if(tempa==-1)
  	return pmem_rw(addr, uint32_t) & (~0u >> ((4 - len) << 3));
	else
	{
//		return mmio_read(addr,len,tempa)&(~0u>>((4-len)<<3));
  	return mmio_read(addr, len,tempa) & (~0u >> ((4 - len) << 3));

	}
}

void paddr_write(paddr_t addr, uint32_t data, int len) {
	int tempa=is_mmio(addr);
	if(tempa==-1)
		memcpy(guest_to_host(addr), &data, len);
	else
		mmio_write(addr, len,data,tempa);
}
//vadde_t==paddr_t==uint32_t
uint32_t vaddr_read(vaddr_t addr, int len) {
//	Log("I am in vaddr_read");
	if (((addr & 0xfff) + len) > 0x1000) 
	{
		/* this is a special case, you can handle it later. */
		Log("In vaddr_read , GG because addr+len>bound,addr:0x%x; len:0x%x",addr,len);
		assert(0);
	}
	else
	{
		paddr_t paddr = page_translate(addr);
		return paddr_read(paddr, len);
	}
//  return paddr_read(addr, len);
}

void vaddr_write(vaddr_t addr, uint32_t data, int len) {
//	Log("I am in vaddr_write");
	if (((addr & 0xfff) + len) > 0x1000) {
		/* this is a special case, you can handle it later. */
		Log("In vaddr_write, GG because addr+len>bound");
		assert(0);
	}
	else{	
		paddr_t paddr = page_translate(addr);
		paddr_write(paddr,data,len);
	}	
	
	
//	paddr_write(addr, data, len);

}

//from nexus-am/am/arch/x86-nemu/include/x86.h
//|		10		|		10		|		12		|
//	PDI				PTI				OFF
//	PDX(va)			PTX(va)			OFF(va)
//#define PDXSHFT 22
//#define PTXSHFT 12
#define PDX(va)		(((uint32_t)(va)>>22)&0x3ff)
#define PTX(va)		(((uint32_t)(va)>>12)&0x3ff)
#define OFF(va)		((uint32_t)(va)&0xfff)






paddr_t page_translate(vaddr_t vaddr){
//	Log("I am in page_translate");
	if(cpu.CR0.PG==0){
	//	Log("I AM PG==0");
//	assert(0);
		return vaddr;//PG=0则直接把段机制产生的线性地址当作物理地址使用
	}
//	assert(0);
	//通过页目录索引DIR以及页目录基地址PDB寻页表基地址
	uint32_t PDB=cpu.CR3.page_directory_base;
	//uint32_t TEMPDIR=(((uint32_t)(vaddr)>>22)&0x3ff);
	uint32_t TEMPDIR=PDX(vaddr);
	uint32_t PDE_page_frame=paddr_read((PDB<<12)+(TEMPDIR<<2),4);
	assert(PDE_page_frame&0x1);//页表或页不在主存中
//	assert(0);
	//通过页表索引PAGE以及页表基地址寻找页表项
//	uint32_t TEMPPAGE=(((uint32_t)(vaddr)>>12)&0x3ff);
	uint32_t TEMPPAGE=PTX(vaddr);
	uint32_t PTE_page_frame=paddr_read((PDE_page_frame&0xfffff000)+(TEMPPAGE<<2),4);
	assert(PTE_page_frame&0x1);//页表或页不在主存中
	
	paddr_t phyaddr=(PTE_page_frame&0xfffff000)+OFF(vaddr);
//	Log("in translate phyaddr:0x%x",phyaddr);
	return phyaddr;
}


